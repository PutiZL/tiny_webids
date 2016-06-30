#ifndef _IDS_CLI_NET_H_
#define _IDS_CLI_NET_H_ 


#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "ids_log.h"

namespace tinyids
{

	
	class IdsCliNet
	{
	public:
		IdsCliNet()
		{
			if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			{
				IdsLog::fatal("Socket created failed, " +std::string(strerror(errno)));
			}
		}

		int32_t get_fd() const
		{
			return connfd;
		}

		bool connect(const struct sockaddr_in &addr);
		bool connect(const std::string &ip, const uint16_t port);
		ssize_t read(void *buf, size_t count);
		ssize_t write(const void *buf, size_t count);
	private:
		int32_t connfd;

	};  /* class IdsCliNet */ 

} /* namespace tinyids */ 



#endif   /* _IDS_CLI_NET_H_ */ 
