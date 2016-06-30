#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include <json/json.h>


#include "ids_epoll.h"
#include "ids_headers.h"

namespace tinyids
{
	IdsEpoll::IdsEpoll(const epoll_conf_t &conf)
	{
		server.reset(new ServAddr(conf.port, conf.if_addr));
		if (!server)
		{
			IdsLog::fatal("Create ServAddr failed!");
		}
	
	}

	
	void IdsEpoll::init()
	{
		IdsLog::info("IdsEpoll init!");
		
		data_center.reset(new IdsDbAdapter(db_conf));
		if (!data_center)
		{
			IdsLog::fatal("IdsDbClient created failed!");
		}
	}


	void IdsEpoll::set_nonblocking(const int32_t fd)
	{
		int flags = fcntl(fd, F_GETFL, 0);
		if (flags == -1)
		{
			IdsLog::fatal("fcntl get flags failed!");
		}
		flags = flags | O_NONBLOCK;
		if (fcntl(fd, F_SETFL, flags) < 0)
		{
			IdsLog::fatal("fcntl set flags failed!");
		}
		IdsLog::info("fcntl set nonblock succeed!");
	}

	void IdsEpoll::start()
	{
		server->set_reuse();
		server->start_listen();
		epollfd = epoll_create(MAXEPOLLSIZE);
		if (epollfd < 0)
		{
			IdsLog::fatal("epoll_create failed, " + std::string(strerror(errno)));
		}
		IdsLog::info("Epoll created succeed!");

		struct epoll_event ev;
		/*ev.events = ::EPOLLIN;*/
		ev.events = ::EPOLLIN | ::EPOLLET;
		ev.data.fd = server->get_fd();

		if (epoll_ctl(epollfd, EPOLL_CTL_ADD, server->get_fd(), &ev) < 0)
		{
			IdsLog::fatal("epoll_ctl add listenfd failed!");	
		}
		IdsLog::info("epoll_ctl add listenfd succeed!");


		start_loop_shedule();	
	}


	void IdsEpoll::start_loop_shedule()
	{
		struct epoll_event events[MAXEPOLLSIZE];
		int32_t nfds;
		for (;;)
		{
			nfds = epoll_wait(epollfd, events, MAXEPOLLSIZE, -1);
			if (nfds < 0)
			{
				IdsLog::fatal("epoll_wait failed!");
			}
			else if (nfds == 0)
				continue;
			else
			{
				for (int32_t i=0; i<nfds; i++)
				{
					if (events[i].data.fd == server->get_fd())
						handle_connection();
					else
					{
						handle_request(events[i].data.fd);	
					}
				}
			}
		}
	}


	/**
	 * @brief: handle connection from peer
	 * @date: Mon Apr 25 12:56:23 CST 2016 
	 * @author: zhuoliang3637@gmail.com
	 * @param: 
	 * @return: 
	 */
	bool IdsEpoll::handle_connection()
	{
		struct sockaddr_in cliaddr;
		socklen_t addrlen = sizeof(cliaddr);
		int connfd = accept(server->get_fd(), (struct sockaddr*)&cliaddr, &addrlen);
		if (connfd < 0)
		{
			IdsLog::fatal("Establish connection from peer failed, " + std::string(strerror(errno)));
		}
		else
		{
			set_nonblocking(connfd);
			struct epoll_event ev;
			ev.data.fd = connfd;
			ev.events = ::EPOLLIN | ::EPOLLET;
			if (epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &ev) < 0)
			{
				IdsLog::fatal("epoll_ctl add failed, " + std::string(strerror(errno)));
			}
			
			IdsLog::info("Connection from " + std::string(inet_ntoa(cliaddr.sin_addr)));
		}
		return true;	
	}


	/**
	 * @brief: handle request from client
	 * @date: Mon Apr 25 12:55:56 CST 2016 
	 * @author: zhuoliang3637@gmail.com
	 * @param: 
	 * @return: 
	 */
	bool IdsEpoll::handle_request(const int32_t fd)
	{
		char buf[2048];
		uint16_t len;
		ssize_t nread;

		while (true)
		{
			nread = ::read(fd, &len, sizeof(len));

			
			if (nread < 0)
			{
				if (errno == EAGAIN)
					return false;
				IdsLog::info("Read failed, client closed!");
				tcpflows.erase(fd);
				::close(fd);
				return false;
			}
			else if (nread == 0)
			{
				IdsLog::info("Read failed, client closed!");
				tcpflows.erase(fd);
				::close(fd);
				return false;
			}
			else
			{
				char packet_len[24];
				snprintf(packet_len, sizeof(packet_len), "%u", len);
				IdsLog::info("Packet len: " + std::string(packet_len));
				while (true)
				{
					nread = ::read(fd, buf, len);
					if (nread < 0)
					{
						if (errno == EAGAIN)
							continue;
						IdsLog::info("client closed!");
						tcpflows.erase(fd);
						::close(fd);
						return false;
					}
					else if (nread == 0)
					{
						IdsLog::info("Client closed!");
						tcpflows.erase(fd);
						::close(fd);
						return false;
					}

					if (nread != len)
					{
						IdsLog::warn("Not enough data!");
						return false;
					}

					IdsLog::info("Received enough data!");	
					size_t offset = 0;
					struct sniff_ethernet *ethernet = (struct sniff_ethernet*)buf;
					offset += sizeof(*ethernet);
					struct sniff_ip *ip = (struct sniff_ip*)(buf + offset);
					int size_ip = IP_HL(ip) * 4;
					offset += size_ip;
					struct sniff_tcp *tcp = (struct sniff_tcp*)(buf + offset);
					int size_tcp = TH_OFF(tcp) * 4;
					offset += size_tcp;
					if ((len - offset) <= 0 || len <= 60)
						break;
					char size_len[24];
					snprintf(size_len, sizeof(size_len), "%ld", len-offset);
					IdsLog::info("Length: " + std::string(size_len));
					IdsTcpFlow::addr_tuple_t addr;
					addr.src_port = tcp->th_sport;
					addr.dst_port = tcp->th_dport;
					addr.src_ip = ip->ip_src.s_addr;
					addr.dst_ip = ip->ip_dst.s_addr;
					tcpflows[fd].append(addr, std::string(buf+offset, len-offset));

					IdsHttp http;
					int32_t ret = http.unpack(tcpflows[fd].get(addr).c_str(), tcpflows[fd].get(addr).length());
					if (ret == 1)
					{
						tcpflows[fd].clear(addr);
						std::string http_info = retrive_http_info(http);
						int32_t size = http_info.length();
						::write(fd, &size, sizeof(size));
						::write(fd, http_info.c_str(), size);
						IdsLog::info("Parse http succeed!");
						IdsLog::info(http_info);
						break;
					}
					else if (ret == 0)
					{
						IdsLog::info("Need more data!");	
						break;
					}
					else
					{
						IdsLog::warn("Parse http failed!");	
						tcpflows[fd].clear(addr);
						break;
					}
				}
			}
		}
		return true;
	}


	std::string IdsEpoll::retrive_http_info(const IdsHttp &http)
	{
		Json::Value http_info;
		Json::FastWriter writer;

		if (http.is_request())
		{
			http_info["TYPE"] = "REQUEST";
			http_info["INFO"] = http.get_request_url();
			if (data_center->is_url_malicious(http.get_request_url()))
			{
				http_info["MALICIOUS"] = "YES";
			}
			else
			{
				http_info["MALICIOUS"] = "NO";
			}
			http_info["METHOD"] = "URL";
		}
		else
		{
			http_info["TYPE"] = "RESPONSE";
			http_info["INFO"] = http.get_file_md5();
			if (data_center->is_md5_malicious(http.get_file_md5()))
			{
				http_info["MALICIOUS"] = "YES";
			}
			else if (http.is_file_pe())
			{
				http_info["MALICIOUS"] = data_center->is_pe_malicious(http.get_body().data(), http.get_body().length());
			}

			else
				http_info["MALICIOUS"] = "NO";
			http_info["METHOD"] = "HASH";
		}

		return writer.write(http_info);
	}
}  /* namespace tinyids */ 


#ifdef TEST_EPOLL
#include "ids_ini.h"
int main()
{
	tinyids::SimpleIni ini;
	ini.LoadFile("ids.ini");
	tinyids::IdsLog::log_conf_t log_conf;
	log_conf.fname = ini.GetValue("Log", "fname");
	tinyids::IdsLog::init(log_conf);	
	tinyids::epoll_conf_t epoll_conf;
	epoll_conf.if_addr = ini.GetValue("Server", "if_addr");
	epoll_conf.port = atoi(ini.GetValue("Server", "port"));
	tinyids::IdsEpoll ids_epoll(epoll_conf);
	ids_epoll.start();
}
#endif /* TEST_EPOLL */
