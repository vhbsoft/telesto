#ifndef SOURCEMASK_H
#define SOURCEMASK_H
#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "filter-element.h"
//destination mask needs further understanding on how this should be implemented.
//such as comparing the destination address regarding to the mask???

namespace ns3{

class SourceMask: public FilterElement{
	protected:
		Ipv4Mask value;
	
	public:
		SourceMask(Ipv4Mask num);
		bool match(Ptr<QueueItem> p);

};

}

#endif
