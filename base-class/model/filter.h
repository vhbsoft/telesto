#ifndef FILTER_H
#define FILTER_H

#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/packet.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <queue>
#include "filter-element.h"


namespace ns3{

class Filter{
	public:
		Filter();
		void set_element(ns3::FilterElement*);
		bool match(Ptr<QueueItem> p);
		std::vector<ns3::FilterElement*> element;

};

}


#endif
