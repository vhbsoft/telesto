/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef STRICT_PRIORITY_QUEUE_H
#define STRICT_PRIORITY_QUEUE_H

#include <queue>
//#include "ns3/ptr.h"
#include <vector>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/base-class.h"
#include "ns3/destination-portnumber.h"


namespace ns3 {

class TraceContainer;

class StrictPriorityQueue : public BaseClass {

  bool DoEnqueue(Ptr<QueueItem> p);
  Ptr<QueueItem> DoDequeue(void);
  Ptr<const QueueItem> DoPeek (void) const;
  // returns 0 for low
  // returns 1 for high 
  uint16_t classify (Ptr<QueueItem> p);
  void schedule(Ptr<QueueItem> p);
  uint32_t m_priorityPort0;
  uint32_t m_priorityPort1;
  uint32_t m_priorityPort2;
  uint32_t m_priorityPort3;
  uint32_t m_priorityPort4;

  uint32_t m_maxPackets0;
  uint32_t m_maxPackets1;
  uint32_t m_maxPackets2;
  uint32_t m_maxPackets3;
  uint32_t m_maxPackets4;

  uint32_t m_maxBytes0;
  uint32_t m_maxBytes1;
  uint32_t m_maxBytes2;
  uint32_t m_maxBytes3;
  uint32_t m_maxBytes4;

  // QueueMode m_mode;
  
  public:
    static TypeId GetTypeId (void);
    void SetMode (StrictPriorityQueue::QueueMode mode);
    StrictPriorityQueue::QueueMode GetMode (void);

    StrictPriorityQueue();
    ~StrictPriorityQueue();

};

} // namespace ns3

#endif /* PRIORITY_QUEUE_H */

