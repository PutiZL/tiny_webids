#include "ids_log.h"

namespace tinyids
{
	std::string IdsLog::name = "tinyids";
	void IdsLog::init(const log_conf_t &conf)
	{
		log4cplus::SharedAppenderPtr pConsole;
		if (conf.fname == "stdout" || conf.fname == "stderr")
		{
			pConsole = log4cplus::SharedAppenderPtr(new log4cplus::ConsoleAppender());	
		}
		else
		{
			pConsole = log4cplus::SharedAppenderPtr(new log4cplus::FileAppender(conf.fname));	
		}
		std::string pattern = "%D [%p]  %m%n";
		pConsole->setLayout(std::unique_ptr<log4cplus::PatternLayout>(new log4cplus::PatternLayout(pattern)));
		log4cplus::Logger logger = log4cplus::Logger::getInstance(name);
		logger.addAppender(pConsole);
	}

	void IdsLog::info(const std::string &msg)
	{
		log4cplus::Logger logger = log4cplus::Logger::getInstance(name);
		logger.log(log4cplus::INFO_LOG_LEVEL, msg);
	}


	void IdsLog::warn(const std::string &msg)
	{
		log4cplus::Logger logger = log4cplus::Logger::getInstance(name);
		logger.log(log4cplus::WARN_LOG_LEVEL, msg);
	}

	void IdsLog::fatal(const std::string &msg)
	{
		log4cplus::Logger logger = log4cplus::Logger::getInstance("tinyids");
		logger.log(log4cplus::FATAL_LOG_LEVEL, msg);
		exit(1);
	}

	std::string operator + (const std::string s, const int32_t i)
	{
		char buf[256];
		snprintf(buf, sizeof(buf), "%d", i);
		return s + std::string(buf);
	}


} /* namespace tinyids*/ 


#ifdef TEST_LOG

#include "ids_ini.h"
int main()
{
	tinyids::SimpleIni ini;
	ini.LoadFile("ids.ini");
	tinyids::IdsLog::log_conf_t log_conf;
	log_conf.fname = ini.GetValue("Log", "fname");
	tinyids::IdsLog::init(log_conf);	
	tinyids::IdsLog::info("This is info");
	tinyids::IdsLog::warn("This is a warning");
	tinyids::IdsLog::fatal("This is a fatal");
	return 0;
}
#endif /* TEST_LOG */
