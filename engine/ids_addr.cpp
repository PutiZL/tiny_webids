#include "ids_addr.h"


namespace tinyids
{
	ServAddr::ServAddr(const uint16_t port, const std::string &if_addr)
	{
		if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		{
			IdsLog::fatal("Socket created failed, " + std::string(strerror(errno)));
		}
		
		IdsLog::info("Socket created succeed!");	
		this->port = port;
		this->if_addr = if_addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = inet_addr(if_addr.c_str());
	}

	bool ServAddr::listen()
	{
		if (::listen(listenfd, 5) < 0)
		{
			IdsLog::fatal("Listen failed, " + std::string(strerror(errno)));
		}
		IdsLog::info(std::string("Listen on port ") + port + ", " + std::string(if_addr));
		return true;
	}

	bool ServAddr::bind() const
	{
		if (::bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
		{
			IdsLog::fatal("Bind failed, " + std::string(strerror(errno)));
		}
		IdsLog::info("Bind succeed!");
		return true;	
	}
} /* namespace tinyids */ 



#ifdef TEST_ADDR
#include "ids_ini.h"

int main()
{
	tinyids::SimpleIni ini;
	ini.LoadFile("ids.ini");
	tinyids::IdsLog::log_conf_t log_conf;
	log_conf.fname = ini.GetValue("Log", "fname");
	tinyids::IdsLog::init(log_conf);	
	tinyids::ServAddr addr(8080, "0.0.0.0");
	addr.start_listen();
	
	return 0;
}

#endif  /* TEST_ADDR */ 
