#ifndef _ENGINE_TINYIDS_H_
#define _ENGINE_TINYIDS_H_

#include "ids_log.h"
#include "ids_epoll.h"
#include "ids_ini.h"

namespace tinyids
{
	class IdsEngine
	{
	public:
		IdsEngine();
		~IdsEngine()
		{
		}

		bool load_config(const std::string &conf_path);
		bool init();
		void run();
	private:
		SimpleIni ini;

		epoll_conf_t epoll_conf;
		IdsDB::db_conf_t db_conf;	
		
		boost::shared_ptr<IdsEpoll> ids_epoll;
		
	};  /* class IdsEngine */ 

} /* namespace tinyids */ 

#endif /* _ENGINE_TINYIDS_H_ */ 
