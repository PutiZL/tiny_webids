#include "ids_db.h"

//#include <algorithm>
#include <boost/algorithm/string.hpp>

namespace tinyids
{
	IdsDB::IdsDB(const db_conf_t& conf)
	{
		driver = sql::mysql::get_driver_instance();
		if (!driver)
		{
			IdsLog::fatal("Cannot get MySQL driver instance!");
		}
		IdsLog::info("Get MySQL driver instance succeed!");

		conn.reset(driver->connect(conf.host, conf.user, conf.passwd));
		if (!conn)
		{
			IdsLog::fatal("Cannot get connection to MySQL!");
		}
		IdsLog::info("Get connection to MySQL server succeed!");

		boost::scoped_ptr<sql::Statement> stat(conn->createStatement());
		if (!stat)
		{
			IdsLog::fatal("Cannot create statement!");		
		}

		if (!changeDB(conf.db))
		{
			IdsLog::fatal("Cannot change database to " + conf.db);	
		}
		IdsLog::info("Change database to " + conf.db);
	}


	bool IdsDB::changeDB(const std::string &db)
	{
		std::string exp = "USE " + db;
		boost::scoped_ptr<sql::Statement> stat(conn->createStatement());
		if (!stat)
		{
			IdsLog::warn("Cannot create statement!");
			return false;
		}

		stat->execute(exp);
		/*return stat->execute(exp);*/
		return true;
	}


	bool IdsDB::execute(const std::string &exp)
	{
		boost::scoped_ptr<sql::Statement> stat(conn->createStatement());
		if (!stat)
		{
			IdsLog::warn("Cannot create statement!");
			return false;
		}
		IdsLog::info("Execute sql, " + exp);
		return stat->execute(exp);
	}


	sql::ResultSet* IdsDB::query(const std::string &exp)
	{
		boost::scoped_ptr<sql::Statement> stat(conn->createStatement());
		if (!stat)
		{
			IdsLog::warn("Cannot create statement!");
			return NULL;
		}
		IdsLog::info("Execute query, " + exp);
		return stat->executeQuery(exp);
	}



	IdsDbClient::IdsDbClient(const std::string &host, const std::string &user, const std::string &passwd, const std::string &db)
	{
		IdsDB::db_conf_t conf = { host, db, user, passwd };
		ids_db.reset(new IdsDB(conf));
		if (!ids_db)
		{
			IdsLog::fatal("Cannot create IdsDB object!");
		}

		tbl_name["nss_exploit"]    = "nss-exploit";
		tbl_name["nss_cve_info"]   = "nss-cve-info";
		tbl_name["nss_detec_file"] = "nss-detec-file";
		IdsLog::info("DB client created succeed!");	
	}


	std::vector<IdsDbClient::nss_exploit_data_t> IdsDbClient::searchNssExploitByNssId(const std::string &nss_id)
	{
		std::vector<nss_exploit_data_t> results;
		std::string exp = "SELECT * FROM `" + tbl_name["nss_exploit"] + "` WHERE nss_id='" + nss_id + "' LIMIT 1";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		if (!result_set)
		{
			IdsLog::warn("No data from database!");
		}
		else
		{
			while (result_set->next())
			{
				nss_exploit_data_t result;
				result.id = result_set->getUInt(1);
				/*result.nss_id = result_set->getString(2);*/
				result.nss_id = nss_id;
				result.detec_time = result_set->getString(3);
				result.tgt_app = result_set->getString(4);
				result.url = result_set->getString(5);
				result.ip = result_set->getString(6);
				result.victim = result_set->getString(7);
				results.push_back(result);
			}
		}
		return results;
	}


	std::vector<IdsDbClient::nss_exploit_data_t> IdsDbClient::searchNssExploitByUrl(const std::string &url)
	{
		std::vector<nss_exploit_data_t> results;
		std::string exp = "SELECT * FROM `" + tbl_name["nss_exploit"] + "` WHERE malicious_url='" + url + "'";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		if (!result_set)
		{
			IdsLog::warn("No data from database!");
		}
		else
		{
			while (result_set->next())
			{
				nss_exploit_data_t result;
				result.id = result_set->getUInt(1);
				result.nss_id = result_set->getString(2);
				result.detec_time = result_set->getString(3);
				result.tgt_app = result_set->getString(4);
				result.url = url;
				/*result.url = result_set->getString(5);*/
				result.ip = result_set->getString(6);
				result.victim = result_set->getString(7);
				results.push_back(result);
			}
		}
		return results;
	}


	std::vector<IdsDbClient::nss_exploit_data_t> IdsDbClient::searchNssExploitByIp(const std::string &ip)
	{
		std::vector<nss_exploit_data_t> results;
		std::string exp = "SELECT * FROM `" + tbl_name["nss_exploit"] + "` WHERE malicious_ip LIKE '" + ip + "%'";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		if (!result_set)
		{
			IdsLog::warn("No data from database!");
		}
		else
		{
			while (result_set->next())
			{
				nss_exploit_data_t result;
				result.id = result_set->getUInt(1);
				result.nss_id = result_set->getString(2);
				result.detec_time = result_set->getString(3);
				result.tgt_app = result_set->getString(4);
				result.url = result_set->getString(5);
				result.ip = ip;
				/*result.ip = result_set->getString(6);*/
				result.victim = result_set->getString(7);
				results.push_back(result);
			}
		}
		return results;
	}




	std::vector<IdsDbClient::nss_cve_data_t> IdsDbClient::searchCVEInfoByNssId(const std::string &nss_id)
	{
		std::vector<nss_cve_data_t> results;
		std::string exp = "SELECT * FROM `" + tbl_name["nss_cve_info"] + "` WHERE nss_id='" + nss_id + "'";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		while (result_set->next())
		{
			nss_cve_data_t 	result;
			result.id = result_set->getUInt(1);
			result.nss_id = nss_id;
			/*result.nss_id = result_set->getString(2);*/
			result.cve_id = result_set->getString(3);
			result.cve_desc = result_set->getString(4);
			results.push_back(result);
		}
		return results;
	}

	std::vector<IdsDbClient::nss_detec_data_t> IdsDbClient::searchDetecFileByNssId(const std::string &nss_id)
	{
		std::vector<nss_detec_data_t> results;
		std::string exp = "SELECT * FROM `" + tbl_name["nss_detec_file"] + "` WHERE nss_id='" + nss_id + "'";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		while (result_set->next())
		{
			nss_detec_data_t result;
			result.id = result_set->getUInt(1);
			result.nss_id = nss_id;
			/*result.nss_id = result_set->getString(2);*/
			result.file_name = result_set->getString(3);
			result.md5 = result_set->getString(4);
			result.sha1 = result_set->getString(5);
			result.sha256 = result_set->getString(6);
			results.push_back(result);
		}
		return results;
	}

	std::vector<IdsDbClient::nss_detec_data_t> IdsDbClient::searchDetecFileByMd5(const std::string &md5)
	{
		std::vector<nss_detec_data_t> results;
		IdsLog::info("MD5 upper!");	
		std::string exp = "SELECT * FROM `" + tbl_name["nss_detec_file"] + "` WHERE md5_hash='" + md5 + "'";
		boost::scoped_ptr<sql::ResultSet> result_set(ids_db->query(exp));
		while (result_set->next())
		{
			nss_detec_data_t result;
			result.id = result_set->getUInt(1);
			result.nss_id = result_set->getString(2);
			result.file_name = result_set->getString(3);
			result.md5 = md5;
			result.sha1 = result_set->getString(5);
			result.sha256 = result_set->getString(6);
			results.push_back(result);
		}
		return results;
	}
} /* _IDS_DB_H_ */


#ifdef TEST_DB
int main()
{
	tinyids::IdsLog::log_conf_t log_conf = { "stdout" };
	tinyids::IdsLog::init(log_conf);

	tinyids::IdsDbClient dbclient("tcp://192.168.136.130:3306", "root", "zhuoliang", "nss");

	std::vector<tinyids::IdsDbClient::nss_exploit_data_t> exploits = dbclient.searchNssExploitByNssId("NSS-2015-DSCZW");
	for (tinyids::IdsDbClient::nss_exploit_data_t exploit : exploits)
	{
		std::cout<<exploit.tostring()<<std::endl;
	}

	exploits = dbclient.searchNssExploitByUrl("http%3A//nyj.longmen.gov.cn/onews.asp%3Fid%3D168");
	for (tinyids::IdsDbClient::nss_exploit_data_t exploit : exploits)
	{
		std::cout<<exploit.tostring()<<std::endl;
	}


	exploits = dbclient.searchNssExploitByIp("121.15.254.11");
	for (tinyids::IdsDbClient::nss_exploit_data_t exploit : exploits)
	{
		std::cout<<exploit.tostring()<<std::endl;
	}

	std::vector<tinyids::IdsDbClient::nss_cve_data_t> cves = dbclient.searchCVEInfoByNssId("NSS-2015-DSCZW");
	for (tinyids::IdsDbClient::nss_cve_data_t cve : cves)
	{
		std::cout<<cve.tostring()<<std::endl;
	}

	std::vector<tinyids::IdsDbClient::nss_detec_data_t> files = dbclient.searchDetecFileByNssId("NSS-2015-DSCZW");
	for (tinyids::IdsDbClient::nss_detec_data_t file : files)
	{
		std::cout<<file.tostring()<<std::endl;
	}
	return 0;
}
#endif /* TEST_DB */
