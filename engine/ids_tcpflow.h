#ifndef _IDS_TCPFLOW_H_
#define _IDS_TCPFLOW_H_

#include <stdint.h>
#include <arpa/inet.h>

#include <string>
#include <list>
#include <sstream>

#include <boost/shared_ptr.hpp>

#include "ids_log.h"

namespace tinyids
{

	class IdsTcpFlow
	{
	public:	

		typedef struct addr_tuple
		{
			uint16_t src_port;
			uint16_t dst_port;
			uint32_t src_ip;
			uint32_t dst_ip;
			std::string tostring() const
			{
				std::stringstream ss;
				ss<<"src_port: "<<ntohs(src_port);
				ss<<", src_ip: "<<ntohl(src_ip);
				ss<<", dst_port: "<<ntohs(dst_port);
				ss<<", dst_ip: "<<ntohl(dst_ip)<<std::endl;
				return ss.str();
			}
		}addr_tuple_t;

		typedef struct tcp_flow
		{
			addr_tuple_t addr;
			std::string buffer;
		}tcp_flow_t;

	public:

		std::string get(const addr_tuple_t &addr);
		void clear(const addr_tuple_t &addr);
		bool append(const addr_tuple_t &addr, const std::string &content);
	private:
		boost::shared_ptr<tcp_flow_t>& hash(const addr_tuple_t &addr);

	private:
		const static int32_t HASH_SIZE = 19;

		std::list<boost::shared_ptr<tcp_flow_t> > flows[HASH_SIZE];
	};  /* class IdsTcpFlow */


} /* namespace tinyids */



#endif /* _IDS_TCPFLOW_H_ */
