#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "destination-ipadress.h"

namespace ns3{

DestinationIPAddress::DestinationIPAddress(Ipv4Address num){
	value = num;
}

bool DestinationIPAddress::match(Ptr<QueueItem> p){
	bool matching = false;
	Ipv4Header ip;
	p->GetPacket()->RemoveHeader(ip);
	matching = (value == ip.GetDestination());
	p->GetPacket()->AddHeader(ip);
	return matching;
}




}
