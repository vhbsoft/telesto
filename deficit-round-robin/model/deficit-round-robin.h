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

  bool Enqueue(Ptr<ns3::Packet> p);
  Ptr<ns3::Packet> Dequeue(void);
  bool DoEnqueue(Ptr<ns3::Packet> p);
  Ptr<ns3::Packet> DoDequeue(void);
  void schedule(Ptr<ns3::Packet> p);
  uint16_t classify(Ptr<ns3::Packet> p);
  Ptr<ns3::Packet>  Remove (void);
  Ptr<const ns3::Packet> Peek (void) const;
  int32_t GetQueueToBePeeked() const;

	uint32_t quantumSize;
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

