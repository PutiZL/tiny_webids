#include "ids_http.h"

#include <stdio.h>

#include <string.h>
#include <sstream>
#include <stdio.h>

#include "zlib.h"
#include "ids_string_util.h"

namespace tinyids
{
	int hex2int(const std::string s)
	{
		IdsLog::info("hex2int: " + s);
		if (s.empty())
			return -1;

		int result = 0;
		for (char c : s)
		{
			if (c >= '0' && c <= '9')
			{
				result = result*16 + c-'0';
			}
			else if (c >= 'a' && c <= 'f')
			{
				result = result*16 + c-'a'+10;
			}
			else if (c >= 'A' && c <= 'F')
			{
				result = result*16 + c-'A'+10;
			}
		}
		return result;
	}

	int32_t IdsHttp::unpack(const char *data, size_t len)
	{
		const char *pos = strstr(data, "\r\n");
		if (pos == NULL)
		{
			IdsLog::warn("Data seems not correct, " + std::string(data, len));
			return -1;
		}

		std::string prelud(data, pos);
		if (!unpack_prelud(prelud))
		{
			IdsLog::warn("Parse prelud failed, " + prelud);
			return -1;
		}

		const char *cur;
		while (true)
		{
			pos += 2;
			cur = pos;
			pos = strstr(cur, "\r\n");
			if (pos == NULL)
			{
				IdsLog::warn("Data seems not correct!");
				return -1;
			}
			std::string line(cur, pos);
			if (line.empty())
			{
				pos += 2;
				if (pos >= data + len)
				{
					/*
					 *if (type == HT_REQ)
					 *{
					 *    return 1;
					 *}
					 *else
					 *{
					 *    IdsLog::warn("Need data!");
					 *    return 0;
					 *}
					 */
					return 1;
				}
				else
				{
					if (type == HT_REQ && method == "GET")
					{
						/*
						 *IdsLog::warn("Extra data!");
						 *return -1;
						 */
						return 1;
					}
					else if(type == HT_REQ && method == "POST")
					{
						std::string post_data(pos, len-(pos-data));

						if (!unpack_data(post_data))
						{
							IdsLog::warn("Parse post data failed, " + post_data);
							return -1;
						}
					}
					else
					{
						std::string body(pos, len-(pos-data));
					    
						if (headers.count("Content-Length"))
						{
							printf("Body length: %ld, Content length: %s\n", body.length(), headers["Content-Length"].c_str());
							if (body.length() < atoi(headers["Content-Length"].c_str()))
							{
								IdsLog::warn("Need more data!");
								return 0;
							}
						}

						if (!unpack_body(body))
						{
							IdsLog::warn("Parse response body failed!");
							return 0;
							//return 1;
						}
					}
				}
				break;
			}
			else
			{
				if (!unpack_header(line))
				{
					IdsLog::warn("Parse header failed, " + line);
					return -1;
				}
			}
		}
		return 1;	
	}	

	bool IdsHttp::unpack_prelud(const std::string &line)
	{
	
		IdsLog::info("Parsing prelud line: " + line);
		
		std::vector<std::string> items = StringUtil::split(line, 2);
		if (items.size() < 3)
		{
			IdsLog::warn("Size seems not correct!");
			return false;
		}

		if (line.find("HTTP") == 0)
		{
			type        = HT_RESP;
			version     = items[0];
			status_code = items[1];
			status_desc = items[2];
		}
		else
		{
			type   = HT_REQ;
			method = items[0];
			path   = items[1];
			version   = items[2];
		}
		return true;	
	}


	bool IdsHttp::unpack_header(const std::string &line)
	{
		IdsLog::info("Parsing header: " + line);
		std::string::size_type colon = line.find_first_of(":");
		if (colon == std::string::npos)
			return false;

		std::string key = line.substr(0, colon);
		std::string val = StringUtil::strip(line.substr(colon+1), " ");
		headers[key] = val;
		
		if (key == "Content-Encoding" && val == "gzip")
		{
			is_gzipped = true;
		}
		
		if (key == "Transfer-Encoding" && val == "chunked")
		{
			is_chunked = true;
		}
		return true;
	}

	bool IdsHttp::unpack_body(const std::string &body)
	{
		IdsLog::info("Parsing body");
			
		if (is_chunked)
		{
			IdsLog::info("Chunked encoding.");
			std::string::size_type pos = 0;
			bool complete_chunk = false;

			while (true)
			{
				std::string::size_type cur = pos;
				pos = body.find("\r\n", pos);
				if (pos == std::string::npos)
				{
					IdsLog::warn("Format seems not correct!");
					return false;
				}
				std::string len(&body[cur], &body[pos]);
				IdsLog::info("Chunk: " + len);
				if (hex2int(len) == 0)
				{
					complete_chunk = true;
					break;
				}
				pos += 2;
				printf("Chunk size: %d\n", hex2int(len));
				this->body += body.substr(pos, hex2int(len));
				pos += hex2int(len);
				pos += 2;
			}
			if (!complete_chunk)
			{
				IdsLog::warn("Incomplete chunked data!");
				return false;
			}
		}
		else
		{
			this->body = body;
		}

		if (is_gzipped)
		{
			IdsLog::info("Uncompress response data!");
			unsigned long size = (body.length() + 12) * 1.2;
			while (true)
			{
				unsigned char *buffer = new unsigned char[size];
				int ret = ::uncompress(buffer, &size, (const unsigned char*)body.c_str(), body.length());
				if (ret != Z_OK)
				{
					if (ret == Z_BUF_ERROR)
					{
						delete []buffer;
						size = size * 2;
						IdsLog::warn("Zip uncompress buffer error!");
						continue;
					}
					else 
					{
						IdsLog::warn("Zlib uncompress failed!");
						break;
					}
				}
				else
				{
					this->body = std::string((const char*)buffer, size);
					delete []buffer;
					IdsLog::info("Zip uncompress succeed!");
				}

			}
		}

		return true;
	}


	bool IdsHttp::unpack_data(const std::string &data)
	{
		IdsLog::info("Parsing post data");
		this->data = data;
		return true;
	}
} /* namespace tinyids */


#ifdef TEST_HTTP

#include <stdio.h>

int main()
{
	using namespace tinyids;
	IdsHttp ids_http;
	IdsLog::log_conf_t log_conf;
	log_conf.fname = "stdout";
	IdsLog::init(log_conf);
	FILE *fp = fopen("ssdut.txt", "r");
	char buffer[231500];
	size_t nread = fread(buffer, 1, sizeof(buffer), fp);
	ids_http.unpack(buffer, nread);
}
#endif /* TEST_HTTP */
