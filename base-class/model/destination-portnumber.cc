#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "destination-portnumber.h"

namespace ns3{

DestinationPortNumber::DestinationPortNumber(uint32_t num){

	value = num;

}

bool DestinationPortNumber::match(Ptr<ns3::Packet> p){

	bool matching = false;
	PppHeader ppp;
	p->RemoveHeader(ppp);
	Ipv4Header ip;
	p->RemoveHeader(ip);
	uint32_t protocol = ip.GetProtocol();
	if (protocol == 17){
		UdpHeader udp;
		p->PeekHeader(udp);
		matching = (value == udp.GetDestinationPort());
	}else if (protocol == 6){
		TcpHeader tcp;
		p->PeekHeader(tcp);
		matching = (value == tcp.GetDestinationPort());
	}else{
		//NS_LOG_INFO("\tFilterElement::match(): unrecognized transport protocol");
	}

	p->AddHeader(ip);
	p->AddHeader(ppp);
	return matching;
}

}
