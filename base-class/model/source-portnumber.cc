#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "source-portnumber.h"

namespace ns3{

SourcePortNumber::SourcePortNumber(uint32_t num){

	value = num;

}


bool SourcePortNumber::match(Ptr<QueueItem> p){

	bool matching = false;
	PppHeader ppp;
	p->GetPacket()->RemoveHeader(ppp);
	Ipv4Header ip;
	p->GetPacket()->RemoveHeader(ip);
	uint32_t protocol = ip.GetProtocol();
	if (protocol == 17){
		UdpHeader udp;
		p->GetPacket()->PeekHeader(udp);
		matching = (value == udp.GetSourcePort());
	}else if (protocol == 6){
		TcpHeader tcp;
		p->GetPacket()->PeekHeader(tcp);
		matching = (value == tcp.GetSourcePort());
	}else{
		//NS_LOG_INFO("\tFilterElement::match(): unrecognized transport protocol");
	}

	p->GetPacket()->AddHeader(ip);
	p->GetPacket()->AddHeader(ppp);
	return matching;
}

}
