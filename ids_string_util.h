#ifndef _IDS_STRING_UTIL_H_
#define _IDS_STRING_UTIL_H_

#include <stdint.h>

#include <string>
#include <sstream>
#include <vector>

namespace tinyids
{
	class StringUtil
	{
	public:
		static std::string strip(const std::string &target, const std::string &chars);		
		static std::vector<std::string> split(const std::string &target, const uint32_t howmany=-1, const char delim=' ');
	}; /* class StringUtil */

} /* namespace tinyids */

#endif /* _IDS_STRING_UTIL_H_ */
