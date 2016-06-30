#include "ids_clinet.h"

namespace tinyids
{

	bool IdsCliNet::connect(const struct sockaddr_in &addr)
	{
		if (::connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			IdsLog::fatal("Failed connect " + std::string(inet_ntoa(addr.sin_addr)));
		}
		IdsLog::info(std::string("Connect succeed! Remote: ") + inet_ntoa(addr.sin_addr) + ":" + addr.sin_port);
		return true;	
	}

	ssize_t IdsCliNet::read(void *buf, size_t count)
	{
		ssize_t nread;
		nread = ::read(connfd, buf, count);
		return nread;
	}

	ssize_t IdsCliNet::write(const void *buf, size_t count)
	{
		ssize_t nwrite;
		nwrite = ::write(connfd, buf, count);
		return nwrite;
	}

	bool IdsCliNet::connect(const std::string &ip, const uint16_t port)
	{
		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		return connect(addr);
	}
} /* namespace tinyids */ 
