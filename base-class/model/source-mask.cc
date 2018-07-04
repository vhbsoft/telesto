#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"

#include "source-mask.h"

namespace ns3{

SourceMask::SourceMask(Ipv4Mask num){

	value = num;

}


bool SourceMask::match(Ptr<QueueItem> p){

	Ipv4Header ip;
	p->GetPacket()->RemoveHeader(ip);
	return 0;
}

}
