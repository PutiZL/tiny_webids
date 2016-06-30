#include "tinyids.h"

#include <unistd.h>

namespace tinyids
{
	IdsEngine::IdsEngine()
	{
	}

	bool IdsEngine::load_config(const std::string &conf_path)
	{
		ini.LoadFile(conf_path.c_str());
		
		IdsLog::log_conf_t log_conf;	
		log_conf.fname = ini.GetValue("Log", "fname");
		IdsLog::init(log_conf);

		epoll_conf.if_addr = ini.GetValue("Server", "if_addr");
		epoll_conf.port = atoi(ini.GetValue("Server", "port"));

		db_conf.host = ini.GetValue("MySQL", "host");
		db_conf.user = ini.GetValue("MySQL", "user");
		db_conf.passwd = ini.GetValue("MySQL", "passwd");
		db_conf.db = ini.GetValue("MySQL", "db");
		return true;
	}

	bool IdsEngine::init()
	{
		IdsLog::info("Init...");
		ids_epoll.reset(new IdsEpoll(epoll_conf));
		if (!ids_epoll)
		{
			IdsLog::fatal("Create IdsEpoll failed!");
		}

		ids_epoll->set_db_conf(db_conf);
		ids_epoll->init();
		return true;
	}


	void IdsEngine::run()
	{
		IdsLog::info("Running...");
		ids_epoll->start();		
	}
}   /* namespace tinyids */ 



#define VERSION "1.1"
#define AUTHOR "zhuoliang3637@gmail.com"
#define TIME "2016.04"

void help()
{
	fprintf(stderr, "Tiny intrusion detection system for linux.\n"
					"\tVersion: %s\n"
					"\tAuthor: %s\n"
					"\tTime: %s\n\n"
					"Usage: tinyidsd [-h] [-c conf_path]\n",
					VERSION, AUTHOR, TIME);
	exit(1);
}


int main(int argc, char **argv)
{
	std::string conf_path("ids.ini");
	int32_t c = -1;

	while ((c = getopt(argc, argv, "hc:")) != -1)
	{
		switch (c)
		{
		case 'h':
			help();
			break;
		case 'c':
			conf_path = optarg;
			break;
		default:
			help();
			break;
		}
	}
	tinyids::IdsEngine ids_engine;
	ids_engine.load_config(conf_path);
	ids_engine.init();
	ids_engine.run();
	return 0;
}
