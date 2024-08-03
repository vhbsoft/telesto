#ifndef TRAFFICCLASS_H
#define TRAFFICCLASS_H
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include <queue>
#include <vector>
#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "filter.h"

namespace ns3{

class TrafficClass{
	protected:
		uint32_t bytes;
		uint32_t packets;
		uint32_t max_bytes;
		uint32_t max_packets;
		double_t weight;  
		uint32_t priority_level;
		bool isDefault;

	public:
		void setBytes(uint32_t num);
		uint32_t getBytes();
		void setPackets(uint32_t num);
		uint32_t getPackets();
		void setMaxPackets(uint32_t num);
		uint32_t getMaxPackets();
		void setMaxBytes(uint32_t num);
		uint32_t getMaxBytes();
		void setWeight(double num);
		double getWeight();
		void setPriorityLevel(uint32_t num);
		uint32_t getPriorityLevel();
		void setIsDefault(bool check);
		bool match(Ptr<QueueItem> p);
		std::queue<Ptr<QueueItem> > m_queue;
		std::vector<ns3::Filter*> filters;
		TrafficClass();

};

}

#endif