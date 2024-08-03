/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef DEFICIT_ROUND_ROBIN_H
#define DEFICIT_ROUND_ROBIN_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include "ns3/base-class.h"
#include "ns3/destination-portnumber.h"

namespace ns3 {

class TraceContainer;

class DeficitRoundRobin: public BaseClass {
public:
	static TypeId GetTypeId(void);

	DeficitRoundRobin();
	~DeficitRoundRobin();

  	void SetMode(DeficitRoundRobin::QueueMode mode);
	DeficitRoundRobin::QueueMode GetMode (void);


private:

	bool DoEnqueue(Ptr<QueueItem> p);
	Ptr<QueueItem> DoDequeue(void);
	Ptr<const QueueItem> DoPeek(void) const;
	uint16_t classify(Ptr<QueueItem> p);
	int32_t GetQueueToBePeeked() const;
	void schedule(Ptr<QueueItem> p);

	uint32_t quantumSize1;
	uint32_t quantumSize2;
	uint32_t quantumSize3;
	uint32_t quantumSize4;
	uint32_t quantumSize5;
	
	uint32_t m_firstPort;
	uint32_t m_secondPort;
	uint32_t m_thirdPort;
	uint32_t m_fourthPort;
	uint32_t m_fifthPort;

	uint32_t m_firstMaxPackets;
	uint32_t m_secondMaxPackets;
	uint32_t m_thirdMaxPackets;
	uint32_t m_fourthMaxPackets;
	uint32_t m_fifthMaxPackets;		

	uint32_t m_firstMaxBytes;
	uint32_t m_secondMaxBytes;
	uint32_t m_thirdMaxBytes;
	uint32_t m_fourthMaxBytes;
	uint32_t m_fifthMaxBytes;

	bool m_serve_queue1;
	bool m_serve_queue2;
	bool m_serve_queue3;
	bool m_serve_queue4;
	bool m_serve_queue5;

	uint16_t  m_first_dc;
	uint16_t  m_second_dc;
	uint16_t  m_third_dc;
	uint16_t  m_fourth_dc;
	uint16_t  m_fifth_dc;

        


};

} // namespace ns3

#endif /* DEFICIT_ROUND_ROBIN_H */

