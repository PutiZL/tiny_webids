#ifndef _IDS_HTTP_H_
#define _IDS_HTTP_H_

#include <map>
#include <string>

#include "ids_log.h"

#include "md5.h"

namespace tinyids
{
	class IdsHttp
	{
	public:
		typedef enum http_type
		{
			HT_REQ,
			HT_RESP
		}http_type_t;

	public:
		IdsHttp()
			:is_chunked(false), is_gzipped(false)
		{}

		~IdsHttp()
		{
		}



		/**
		 * @brief: unpack http data
		 * @date: Mon May  9 12:48:35 CST 2016 
		 * @author: zhuoliang3637@gmail.com
		 * @param: @data packet, @len length of packet
		 * @return: 1 on succeed, 0 on failed and need more data, -1 on failed and clear buffer
		 */
		int32_t unpack(const char *data, size_t len);

		std::string get_request_url() const
		{
			std::string url;
			if (headers.find("Host") != headers.end())
			{
				url += headers.at("Host");
				url += path;
			}
			
			if (url.find("http") != 0)
			{
				url = "http://" + url;
			}
			return url;
		}


		std::string get_file_md5() const
		{
			return MD5(this->body).toStr();
		}

		bool is_file_pe() const
		{
			if (this->body.find("This program cannot be run in DOS mode.") != std::string::npos || this->body.find("This program must be run under Win32") != std::string::npos)
				return true;
			else
				return false;		
		}
		bool is_request() const
		{
			return type == HT_REQ;
		}

		std::string get_body() const
		{
			return body;
		}
	private:
		bool unpack_prelud(const std::string &line);
		bool unpack_header(const std::string &line);
		bool unpack_body(const std::string &body);
		bool unpack_data(const std::string &data);

	private:

		http_type_t type;
		std::string version;
		/* Request */
		std::string method;
		std::string path;

		/* Response */
		std::string status_code;
		std::string status_desc;

		std::map<std::string, std::string> headers;

		std::string body; /* Response */ 
		std::string data; /* Request */

		bool is_chunked;
		bool is_gzipped;
	}; /* class IdsHttp */

} /* namespace tinyids */
#endif /* _IDS_HTTP_H_ */
