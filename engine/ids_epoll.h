#ifndef _ENGINE_IDS_EPOLL_H_
#define _ENGINE_IDS_EPOLL_H_

#include <sys/epoll.h>   /* For epoll */ 

#include <map>

#include <boost/shared_ptr.hpp>

#include "ids_log.h"
#include "ids_addr.h"
#include "ids_http.h"
#include "ids_db_adapter.h"
#include "ids_tcpflow.h"

namespace tinyids
{
	/**
	 * @brief: linux epoll wrapped
	 * @date: Mon Apr 25 01:47:36 PDT 2016 
	 * @author: zhuoliang3637@gmail.com
	 */

	typedef struct epoll_conf
	{
		uint16_t port;
		std::string if_addr;
	}epoll_conf_t;



	class IdsEpoll
	{
	public:	
		IdsEpoll(const epoll_conf_t &conf);
		~IdsEpoll()
		{
		}

		void set_db_conf(const IdsDB::db_conf_t &db_conf)
		{
			this->db_conf = db_conf;
		}

		void init();
		void start();

	private:
		void start_loop_shedule();
		bool handle_connection();
		bool handle_request(const int32_t fd);
		static void set_nonblocking(const int32_t fd);

	private:
		std::string retrive_http_info(const IdsHttp &http);				
	private:
		const static int32_t MAXEPOLLSIZE = 1024;
		int32_t epollfd;	
		boost::shared_ptr<ServAddr> server;
		IdsDB::db_conf_t db_conf;
		boost::shared_ptr<IdsDbAdapter> data_center;
		std::map<int32_t, IdsTcpFlow> tcpflows;
	}; /* class IdsEpoll */ 

} /* namespace tinyids */ 


#endif  /* _ENGINE_IDS_EPOLL_H_ */ 
