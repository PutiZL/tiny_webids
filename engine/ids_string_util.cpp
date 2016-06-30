
#include "ids_string_util.h"

//#include <boost/algorithm/string.hpp>

namespace tinyids
{
	std::string StringUtil::strip(const std::string &target, const std::string &chars)
	{
		if (target.length() == 0)
			return target;
		std::string::size_type begin = target.find_first_not_of(chars);
		std::string::size_type end = target.find_last_not_of(chars) + 1;
		return std::string(&target[begin], &target[end]);
	}


	std::vector<std::string> StringUtil::split(const std::string &target, const uint32_t howmany, const char delim)
	{
		std::vector<std::string> strs;

		if (howmany == -1)
		{
			std::stringstream ss(target);
			std::string item;
			while (std::getline(ss, item, delim))
			{
				strs.push_back(item);
			}
		}
		else
		{
			uint32_t count = 0;
			std::string temp = target;
			std::string::size_type pos = 0;
			while (count < howmany)
			{
				pos = temp.find_first_of(delim);
				if (pos == std::string::npos)
					break;
				strs.push_back(temp.substr(0, pos));
				temp = temp.substr(pos+1);
				count += 1;
			}
			strs.push_back(temp);
		}
		return strs;
	}

} /* namespace tinyids */

#ifdef TEST_STRING_UTIL
#include <iostream>

int main()
{
	std::string target("   huwfwef&&&");
	std::string chars(" &");
	std::cout<<tinyids::StringUtil::strip(target, chars)<<std::endl;
	target = "    ";
	std::cout<<tinyids::StringUtil::strip(target, chars)<<std::endl;
	target = "I love u ";
	std::vector<std::string> strs = tinyids::StringUtil::split(target);
	std::for_each(strs.begin(), strs.end(), [](std::string s){std::cout<<s<<std::endl;});
}

#endif /* TEST_STRING_UTIL */
