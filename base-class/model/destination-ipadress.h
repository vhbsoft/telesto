#ifndef DESTINATIONIPADDRESS_H
#define DESTINATIONIPADDRESS_H

#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "filter-element.h"

namespace ns3{

class DestinationIPAddress: public FilterElement{

	protected:	
		Ipv4Address value;

	public:
		DestinationIPAddress(Ipv4Address num);
		bool match(Ptr<QueueItem> p);
};




}

#endif


