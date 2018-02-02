#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"

#include "destination-mask.h"

namespace ns3{

DestinationMask::DestinationMask(Ipv4Mask num){
	value = num;

}


bool DestinationMask::match(Ptr<ns3::Packet> p){
	Ipv4Header ip;
	p->RemoveHeader(ip);
	return 0;
}



}
