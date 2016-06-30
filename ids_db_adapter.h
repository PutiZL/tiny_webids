#ifndef _IDS_DB_ADAPTER_H_
#define _IDS_DB_ADAPTER_H_


#include <string>
#include <vector>

#include <curl/curl.h>

#include <boost/shared_ptr.hpp>

#include "ids_db.h"
#include "ids_log.h"

namespace tinyids
{

	class IdsDbAdapter
	{
	public:
		IdsDbAdapter(const IdsDB::db_conf_t &conf);
		~IdsDbAdapter()
		{
		}

		bool is_url_malicious(const std::string &url) const;
		bool is_md5_malicious(const std::string &url) const;	
		std::string is_pe_malicious(const char *data, const uint32_t len);	
		static std::string urldecode(const std::string &url);
		static std::string urlencode(const std::string &md5);
		static std::string toupper(const std::string &s);

	private:
		boost::shared_ptr<IdsDbClient> dbclient;	
	}; /* class IdsDbAdapter */

} /* namespace tinyids */





#endif /* _IDS_DB_ADAPTER_H_ */
