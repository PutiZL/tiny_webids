#ifndef _IDS_LOG_H_
#define _IDS_LOG_H_


#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/logger.h>

namespace tinyids
{

	std::string operator + (const std::string s, const int32_t i);
	/*std::string operator + (const char *s1, const std::string s2);*/
	//std::string operator + (const std::string s, const int32_t i);

	class IdsLog
	{
	public:
		typedef struct log_conf
		{
			std::string fname;
		}log_conf_t;

	public:
		static void init(const log_conf_t &conf);
		static void info(const std::string &msg);
		static void warn(const std::string &msg);
		static void fatal(const std::string &msg);

	private:
		static std::string name;		
	};
}   /* namespace tinyids */ 

#endif /* _IDS_LOG_H_ */
