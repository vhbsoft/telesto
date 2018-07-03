#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "protoco-number.h"

namespace ns3{

ProtocolNumber::ProtocolNumber(uint32_t num){
	value = num;

}

bool ProtocolNumber::match(Ptr<ns3::Packet> p){
	Ipv4Header ip;
	p->RemoveHeader(ip);
	return (value == ip.GetProtocol());
}

}
