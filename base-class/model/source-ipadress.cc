#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "source-ipaddress.h"

namespace ns3{

SourceIPAddress::SourceIPAddress(Ipv4Address num){
	
	value = num;

}


bool SourceIPAddress::match(Ptr<ns3::Packet> p){
	bool matching = false;
	Ipv4Header ip;
	p->RemoveHeader(ip);
	matching = (value == ip.GetSource());
	p->AddHeader(ip);
	return matching;
}

}
