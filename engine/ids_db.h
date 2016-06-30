#ifndef _IDS_DB_H_
#define _IDS_DB_H_

#include <stdint.h>

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "ids_log.h"

namespace tinyids
{
	/**
	 * @brief: 
	 * @date: Wed May  4 18:09:37 CST 2016 
	 * @author: zhuoliang3637@gmail.com
	 */
	class IdsDB
	{
	public:
		typedef struct db_conf
		{
			std::string host; /* tcp://127.0.0.1:3306 */
			std::string db;
			std::string user;
			std::string passwd;
		} db_conf_t;
						
	public:
		IdsDB(const db_conf_t &conf);
		~IdsDB()
		{
		}
		bool execute(const std::string &exp);
		sql::ResultSet* query(const std::string &exp);	

	private:
		IdsDB(const IdsDB&);
		IdsDB& operator= (const IdsDB&);

		bool changeDB(const std::string &db);
	private:
		boost::shared_ptr<sql::Connection> conn;
		sql::mysql::MySQL_Driver *driver;
	}; /* class IdsDb */



	/**
	 * @brief: 
	 * @date: Thu May  5 09:23:10 CST 2016 
	 * @author: zhuoliang3637@gmail.com
	 */
	class IdsDbClient
	{
	public:
		typedef struct nss_exploit_data
		{
			uint32_t id;
			std::string nss_id;
			std::string detec_time;
			std::string tgt_app;
			std::string url;
			std::string ip;
			std::string victim;

#ifdef TEST_DB
			std::string tostring() const 
			{
				std::stringstream ss;
				ss<<"ID: "<<id<<std::endl;
				ss<<"NSSId: "<<nss_id<<std::endl;
				ss<<"Detection Time: "<<detec_time<<std::endl;
				ss<<"Target Appliocation: "<<tgt_app<<std::endl;
				ss<<"Malicious URL: "<<url<<std::endl;
				ss<<"Malicious IP: "<<ip<<std::endl;
				ss<<"Victim: "<<victim;
				return ss.str();
			}

#endif /* TEST_DB */
		} nss_exploit_data_t;


		typedef struct nss_cve_data
		{
			uint32_t id;
			std::string nss_id;
			std::string cve_id;
			std::string cve_desc;

#ifdef TEST_DB
			std::string tostring() const
			{
				std::stringstream ss;
				ss<<"ID: "<<id<<std::endl;
				ss<<"NSSId: "<<nss_id<<std::endl;
				ss<<"CVEId: "<<cve_id<<std::endl;
				ss<<"CVE Description: "<<cve_desc;
				return ss.str();
			}
#endif /* TEST_DB */
		} nss_cve_data_t;


		typedef struct nss_detec_data
		{
			uint32_t id;
			std::string nss_id;
			std::string file_name;
			std::string md5;
			std::string sha1;
			std::string sha256;

#ifdef TEST_DB
			std::string tostring() const
			{
				std::stringstream ss;
				ss<<"ID: "<<id<<std::endl;
				ss<<"NSSId: "<<nss_id<<std::endl;
				ss<<"File Name: "<<file_name<<std::endl;
				ss<<"MD5: "<<md5<<std::endl;
				ss<<"SHA1: "<<sha1<<std::endl;
				ss<<"SHA256: "<<sha256;
				return ss.str();
			}
#endif /* TEST_DB */
		} nss_detec_data_t;

	public:
		IdsDbClient(const std::string &host, const std::string &user, const std::string &passwd, const std::string &db);
		~IdsDbClient()
		{
		}

		std::vector<nss_exploit_data_t> searchNssExploitByNssId(const std::string &nss_id);
		std::vector<nss_exploit_data_t> searchNssExploitByUrl(const std::string &url);
		std::vector<nss_exploit_data_t> searchNssExploitByIp(const std::string &ip);

		std::vector<nss_cve_data_t> searchCVEInfoByNssId(const std::string &nss_id);
		std::vector<nss_detec_data_t> searchDetecFileByNssId(const std::string &nss_id);	
		std::vector<nss_detec_data_t> searchDetecFileByMd5(const std::string &md5);	
	private:
		boost::shared_ptr<IdsDB> ids_db;
		std::map<std::string, std::string> tbl_name;		

	}; /* class IdsDbClient */
}/* namespace tinyids */

#endif  /* _IDS_DB_H_ */ 
