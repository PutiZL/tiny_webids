
#include "ids_tcpflow.h"


namespace tinyids
{
	bool operator == (const IdsTcpFlow::addr_tuple_t &one, const IdsTcpFlow::addr_tuple_t &other)
	{
		return one.src_port == other.src_port && one.src_ip == other.src_ip && one.dst_port == other.dst_port && one.dst_ip == other.dst_ip;
	}


	std::string IdsTcpFlow::get(const addr_tuple_t &addr)
	{
		return hash(addr)->buffer;	
	}


	void IdsTcpFlow::clear(const addr_tuple_t &addr)
	{
		hash(addr)->buffer.clear();
	}

	bool IdsTcpFlow::append(const addr_tuple_t &addr, const std::string &content)
	{
		IdsLog::info("append, " + addr.tostring());
		hash(addr)->buffer += content;
		//IdsLog::info(hash(addr)->buffer);
		return true;
	}

	boost::shared_ptr<IdsTcpFlow::tcp_flow_t>& IdsTcpFlow::hash(const addr_tuple_t &addr)
	{
		int32_t index = ((addr.src_port & 0xff) | ( ( addr.dst_port & 0xff ) << 8)  | ( (addr.src_ip & 0xff ) << 16 ) \
						| ( (addr.dst_ip & 0xff ) << 24 )) % HASH_SIZE;

		for (boost::shared_ptr<tcp_flow_t> &flow: flows[index])
		{
			if (flow->addr == addr)
				return flow;
		}

		boost::shared_ptr<tcp_flow_t> flow(new tcp_flow_t);
		flow->addr = addr;

		flows[index].push_front(flow);
		return flows[index].front();
	}

} /* namespace tinyids */
