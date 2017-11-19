#include "ns3/uinteger.h"
#include "ns3/ipv4-header.h"
#include "protoco-number.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/queue.h"
#include <queue>

#include "filter.h"

namespace ns3{

Filter::Filter(){



}



void Filter::set_element(ns3::FilterElement* filter_ele){

	element.push_back(filter_ele);

}


bool Filter::match(Ptr<QueueItem> p){

	for(unsigned int i=0; i<element.size(); i++){
		if(element[i]->match(p) == false)
			return false;
	}
	
	return true;

}


}
