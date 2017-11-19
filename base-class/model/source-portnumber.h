#ifndef SOURCEPORTNUMBER_H
#define SOURCEPORTNUMBER_H
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include "filter-element.h"

namespace ns3{

class SourcePortNumber : public FilterElement{
	protected:
		uint32_t value;
	
	public:
		SourcePortNumber(uint32_t num);
		bool match(Ptr<QueueItem> p);

};


}
#endif