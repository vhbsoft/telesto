#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include <queue>

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/queue.h"
#include "traffic-class.h"

namespace ns3{

TrafficClass::TrafficClass(){

}


void TrafficClass::setBytes(uint32_t num){
	this->bytes = num;

}

uint32_t TrafficClass::getBytes(){
	return this->bytes;

}

void TrafficClass::setPackets(uint32_t num){
	this->packets = num;

}

uint32_t TrafficClass::getPackets(){
	return this->packets;

}

void TrafficClass::setMaxPackets(uint32_t num){
	this->max_packets = num;

}

uint32_t TrafficClass::getMaxPackets(){
	return this->max_packets;

}


void TrafficClass::setMaxBytes(uint32_t num){
	this->max_bytes = num;

}

uint32_t TrafficClass::getMaxBytes(){
	return this->max_bytes;

}

void TrafficClass::setWeight(double num){
	this->weight = num;

}

double TrafficClass::getWeight(){
	return this->weight;

}

void TrafficClass::setPriorityLevel(uint32_t num){
	this->priority_level = num;

}

uint32_t TrafficClass::getPriorityLevel(){
	return this->priority_level;

}

void TrafficClass::setIsDefault(bool check){
	this->isDefault = check;

}

bool TrafficClass::match(Ptr<QueueItem> p){
	bool check = true;
	for(unsigned int i=0; i<filters.size(); i++){
		for(unsigned int j=0; j<filters[i]->element.size(); j++){
			if(filters[i]->match(p) == false){
				check = false;
				break;
			}	
		}
		if(check)
			return true;
	}
	
	return false;
}


}//ns3 namespace
