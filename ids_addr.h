#ifndef _ENGINE_IDS_ADDR_H_
#define _ENGINE_IDS_ADDR_H_

#include <stdint.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#include <string>
#include <sstream>

#include "ids_log.h"

namespace tinyids
{

	/**
	 * @brief: server address
	 * @date: Mon Apr 25 01:54:13 PDT 2016 
	 * @author: zhuoliang@mail.dlut.edu.cn
	 */

	class ServAddr
	{
	public:
		ServAddr(const uint16_t port, const std::string &if_addr);

		ServAddr()
		{
			if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				IdsLog::fatal("Socket created failed, " + std::string(strerror(errno)));
			}
			memset(&addr, 0, sizeof(addr));
		}


		int32_t get_fd() const
		{
			return listenfd;
		}

		void set_port(const uint16_t port) 
		{
			this->port = port;	
		}

		void set_ifaddr(const std::string &if_addr)
		{
			this->if_addr = if_addr;
		}
		
		bool start_listen()
		{
			bind();
			listen();
			return true;
		}

		struct sockaddr_in get_addr() const
		{
			return addr;
		}

		void set_reuse()
		{
			int opt = 1;	
			if ( setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
			{
				IdsLog::fatal("setsockopt failed!")	;
			}
		}
	private:
		/**
		 * @brief: listen 
		 * @date: Mon Apr 25 02:10:17 PDT 2016 
		 * @author: zhuoliang@mail.dlut.edu.cn
		 */
		bool listen();

		bool bind() const;
	private:
		uint16_t port;
		std::string if_addr;
		int32_t listenfd;
		struct sockaddr_in addr;

	}; /* class ServAddr */ 

} /* namespace tinyids */ 


#endif  /* _ENGINE_IDS_ADDR_H_ */ 
