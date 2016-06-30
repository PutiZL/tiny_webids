
#include "ids_db_adapter.h"


#include <algorithm>
#include <cctype>


#include "ids_clinet.h"

namespace tinyids
{
	IdsDbAdapter::IdsDbAdapter(const IdsDB::db_conf_t &conf)
	{
		dbclient.reset(new IdsDbClient(conf.host, conf.user, conf.passwd, conf.db));
		if (!dbclient)
		{
			IdsLog::fatal("Cannot create IdsDbClient!");	
		}
	}
	

	std::string IdsDbAdapter::urlencode(const std::string &url)
	{
		IdsLog::info("urlencode: " + url);
		CURL *curl = curl_easy_init();
		char *encoded_url = curl_easy_escape(curl, url.c_str(), url.length());
		std::string ret = std::string(encoded_url);
		curl_free(encoded_url);
		curl_easy_cleanup(curl);
		IdsLog::info("urlencode done, result: " + ret);
		return ret;

	}


	std::string IdsDbAdapter::urldecode(const std::string &url)
	{
		IdsLog::info("urldecode: " + url);
		CURL *curl = curl_easy_init();
		char *decoded_url = curl_easy_unescape(curl, url.c_str(), url.length(), NULL);
		std::string ret = std::string(decoded_url);
		curl_free(decoded_url);
		curl_easy_cleanup(curl);
		IdsLog::info("urldecode done, result: " + ret);
		return ret;
	}


	std::string IdsDbAdapter::toupper(const std::string &s)
	{
		std::string ret = s;
		std::transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
		return ret;
	}

	bool IdsDbAdapter::is_url_malicious(const std::string &url) const
	{
		std::vector<IdsDbClient::nss_exploit_data_t> exploits  = dbclient->searchNssExploitByUrl(urlencode(url));
		return !exploits.empty();
	}

	bool IdsDbAdapter::is_md5_malicious(const std::string &md5) const
	{
		std::vector<IdsDbClient::nss_detec_data_t> exploits = dbclient->searchDetecFileByMd5(toupper(md5));
		return !exploits.empty();
	}

	std::string IdsDbAdapter::is_pe_malicious(const char *data, const uint32_t len)
	{
		char buffer[16];
		IdsCliNet  cli;
		cli.connect("192.168.136.134", 8080);
		cli.write(data, len);
		ssize_t nread = cli.read(buffer, 16);
		return std::string(buffer, nread);
	}
} /* namespace tinyids */
