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
	uint32_t m_priorityPort;

	uint32_t m_highMaxPackets;
	uint32_t m_lowMaxPackets;

	uint32_t m_highMaxBytes;
	uint32_t m_lowMaxBytes;

	bool m_serve_queue2;
	uint16_t  m_first_dc;
	uint16_t  m_second_dc;
        


};

} // namespace ns3

#endif /* DEFICIT_ROUND_ROBIN_H */

