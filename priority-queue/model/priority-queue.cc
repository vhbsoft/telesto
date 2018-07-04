/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "priority-queue.h"

#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <iostream>

NS_LOG_COMPONENT_DEFINE ("StrictPriorityQueue");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (StrictPriorityQueue);

//Strict Priority Queue constructor 

StrictPriorityQueue::StrictPriorityQueue() : BaseClass(){
  this->m_highMaxPackets = 100;
  this->m_lowMaxPackets = 100;
  this->m_highMaxBytes = 100*65535;
  this->m_lowMaxBytes = 100*65535;
  this->m_priorityPort = 3000;


  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());


  q_class[0]->setBytes(0);                    //initialize
  q_class[1]->setBytes(0);                    //initialize

  q_class[0]->setMaxPackets(m_highMaxPackets);//Custom settings
  q_class[1]->setMaxPackets(m_lowMaxPackets); //Custom settings
  q_class[0]->setMaxBytes(m_highMaxBytes);    //Custom settings
  q_class[1]->setMaxBytes(m_lowMaxBytes);     //Custom settings

  DestinationPortNumber * prioPN = new DestinationPortNumber(m_priorityPort); 

  Filter * fil = new Filter();
  fil->set_element(prioPN);

  q_class[0]->filters.push_back(fil);


  NS_LOG_FUNCTION_NOARGS ();
}

StrictPriorityQueue::~StrictPriorityQueue ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

TypeId 
StrictPriorityQueue::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::StrictPriorityQueue")
     .SetParent<BaseClass>()
     .AddConstructor<StrictPriorityQueue>()
     .AddAttribute ("SPQMode",
                    "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_MODE_PACKETS),
                   MakeEnumAccessor (&StrictPriorityQueue::SetMode),
                   MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES",
                                    QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    .AddAttribute ("HighPriorityMaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_highMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("LowPriorityMaxPackets",
                   "The maximum number of packets accepted by the low priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_lowMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("HighPriorityMaxBytes",
                   "The maximum number of bytes accepted by the high priority queue.",
                   UintegerValue (100 * 65535),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_highMaxBytes),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("LowPriorityMaxBytes",
                   "The maximum number of bytes accepted by the low priority queue.",
                   UintegerValue (100 * 65535),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_lowMaxBytes),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("HighPriorityPort",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3000),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort),
                   MakeUintegerChecker<uint32_t> ());

  return tid;
}



void
StrictPriorityQueue::SetMode (StrictPriorityQueue::QueueMode mode)
{
  NS_LOG_FUNCTION (mode);
  this->m_mode = mode;
}

StrictPriorityQueue::QueueMode
StrictPriorityQueue::GetMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return this->m_mode;
}

uint16_t
StrictPriorityQueue::classify (Ptr<QueueItem> p)
{

  uint16_t priority;
  if(q_class[0]->filters[0]->element[0]->match(p))
      priority = 1;
  else 
      priority = 0;




  return priority;
}

bool
StrictPriorityQueue::DoEnqueue (Ptr<QueueItem> p)
{
  NS_LOG_FUNCTION (this << p);

  uint16_t priority = classify (p);

  // High Priority
  if (priority == 0)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[0]->m_queue.size() >= q_class[0]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ-High: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[0]->getBytes() + p->GetPacket()->GetSize() >= q_class[0]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ-High: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      NS_LOG_LOGIC ("------------------------ENQ High ----------------");
      q_class[0]->setBytes(q_class[0]->getBytes() + p->GetPacket()->GetSize());
      q_class[0]->m_queue.push(p);
      
      //NS_LOG_LOGIC ("SPQ: Number packets " << q_class[0]->m_queue.size());
      //NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[0]->getBytes());
      
      return true; 
    }

  // Low Priority
  else if (priority == 1)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[1]->m_queue.size() >= q_class[1]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ-Low: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[1]->getBytes() + p->GetPacket()->GetSize() >= q_class[1]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ-Low: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      NS_LOG_LOGIC ("----------------------------------------------ENQ LOW ----------------");
      q_class[1]->setBytes( q_class[1]->getBytes() + p->GetPacket()->GetSize ());
      q_class[1]->m_queue.push (p);
      
      //NS_LOG_LOGIC ("SPQ: Number packets " << q_class[1]->m_queue.size ());
      //NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[1]->getBytes());
      
      return true;      
    }

  // This normally never happens unless Classify() has been changed
  else 
    {
      return false;
    }
}

Ptr<QueueItem>
StrictPriorityQueue::DoDequeue (void)
{
  NS_LOG_FUNCTION (this);
  
  if (!q_class[0]->m_queue.empty ())
    {
      Ptr<QueueItem> p = q_class[0]->m_queue.front ();
      q_class[0]->m_queue.pop ();
      q_class[0]->setBytes( (q_class[0]->getBytes()) - (p->GetPacket()->GetSize()));
      
      NS_LOG_LOGIC ("SPQ-High: Popped " <<Simulator::Now ().GetSeconds ()<< " " << p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[0]->m_queue.size());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[0]->getBytes());
      
      return new QueueItem(p->GetPacket());
    }
  else
    { 
      NS_LOG_LOGIC ("SPQ-low: High priority queue empty, serving low priority queue");

      if (!q_class[1]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[1]->m_queue.front ();
          q_class[1]->m_queue.pop ();
          q_class[1]->setBytes( (q_class[1]->getBytes()) - (p->GetPacket()->GetSize()));
          
          NS_LOG_LOGIC ("SPQ-Low: Popped " <<Simulator::Now ().GetSeconds ()<< " " << p);
          
          NS_LOG_LOGIC ("SPQ: Number packets " << q_class[1]->m_queue.size());
          NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[1]->getBytes());
          
          return  new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("SPQ: Low priority queue empty---------------");
          return 0;
        } 
    } 
}

void 
StrictPriorityQueue::schedule(Ptr<QueueItem> p){};

Ptr<const QueueItem>
StrictPriorityQueue::DoPeek (void) const
{
  NS_LOG_FUNCTION (this);
  
  if (!q_class[0]->m_queue.empty ())
    {
      Ptr<QueueItem> p = q_class[0]->m_queue.front ();
      
      //NS_LOG_LOGIC ("Number packets " << q_class[0]->m_queue.size());
      //NS_LOG_LOGIC ("Number bytes " << q_class[0]->getBytes());
      
      return new QueueItem(p->GetPacket());
    }
  else
    {
      //NS_LOG_LOGIC ("High priority queue empty");

      if (!q_class[1]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[1]->m_queue.front ();
          
          //NS_LOG_LOGIC ("Number packets " << q_class[1]->m_queue.size());
          //NS_LOG_LOGIC ("Number bytes " << q_class[1]->getBytes());
          
          return new QueueItem(p->GetPacket());
        }
      else
        {
          //NS_LOG_LOGIC ("Low priority queue empty");
          return 0;
        }
    }
}
// bool 
// StrictPriorityQueue::Enqueue(Ptr<QueueItem> p){
//   return DoEnqueue(p);
// }
// Ptr<QueueItem> 
// StrictPriorityQueue::Dequeue(void){
//   return DoDequeue();
// }
// Ptr<const QueueItem> 
// StrictPriorityQueue::Peek(void) const{
//   return DoPeek();
// }

} // namespace ns3
