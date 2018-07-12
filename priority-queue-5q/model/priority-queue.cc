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
  this->m_maxPackets0 = 100;
  this->m_maxPackets1 = 100;
  this->m_maxPackets2 = 100;
  this->m_maxPackets3 = 100;
  this->m_maxPackets4 = 100;

  this->m_maxBytes0 = 100*65535;
  this->m_maxBytes1 = 100*65535;
  this->m_maxBytes2 = 100*65535;
  this->m_maxBytes3 = 100*65535;
  this->m_maxBytes4 = 100*65535;

  this->m_priorityPort0 = 3000;
  this->m_priorityPort1 = 3001;
  this->m_priorityPort2 = 3002;
  this->m_priorityPort3 = 3003;
  this->m_priorityPort4 = 3004;



  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());
  q_class.push_back(new TrafficClass());


  q_class[0]->setBytes(0);                    //initialize
  q_class[1]->setBytes(0);                    //initialize
  q_class[2]->setBytes(0);                    //initialize
  q_class[3]->setBytes(0);                    //initialize
  q_class[4]->setBytes(0);                    //initialize


  q_class[0]->setMaxPackets(m_maxPackets0);//Custom settings
  q_class[1]->setMaxPackets(m_maxPackets1);//Custom settings
  q_class[2]->setMaxPackets(m_maxPackets2);//Custom settings
  q_class[3]->setMaxPackets(m_maxPackets3);//Custom settings
  q_class[4]->setMaxPackets(m_maxPackets4);//Custom settings

  q_class[0]->setMaxBytes(m_maxBytes0);    //Custom settings
  q_class[1]->setMaxBytes(m_maxBytes1);    //Custom settings
  q_class[2]->setMaxBytes(m_maxBytes2);    //Custom settings
  q_class[3]->setMaxBytes(m_maxBytes3);    //Custom settings
  q_class[4]->setMaxBytes(m_maxBytes4);    //Custom settings

  DestinationPortNumber * prioPN0 = new DestinationPortNumber(m_priorityPort0);
  DestinationPortNumber * prioPN1 = new DestinationPortNumber(m_priorityPort1); 
  DestinationPortNumber * prioPN2 = new DestinationPortNumber(m_priorityPort2); 
  DestinationPortNumber * prioPN3 = new DestinationPortNumber(m_priorityPort3); 
  DestinationPortNumber * prioPN4 = new DestinationPortNumber(m_priorityPort4); 
 
  Filter * fil0 = new Filter();
  Filter * fil1 = new Filter();
  Filter * fil2 = new Filter();
  Filter * fil3 = new Filter();
  Filter * fil4 = new Filter();

  fil0->set_element(prioPN0);
  fil1->set_element(prioPN1);
  fil2->set_element(prioPN2);
  fil3->set_element(prioPN3);
  fil4->set_element(prioPN4);

  q_class[0]->filters.push_back(fil0);
  q_class[1]->filters.push_back(fil1);
  q_class[2]->filters.push_back(fil2);
  q_class[3]->filters.push_back(fil3);
  q_class[4]->filters.push_back(fil4);


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
    .AddAttribute ("Priority0MaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_maxPackets0),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority1MaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_maxPackets1),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority2MaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_maxPackets2),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority3MaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_maxPackets3),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority4MaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_maxPackets4),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("Priority0Port",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3000),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort0),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority1Port",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3001),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort1),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority2Port",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3002),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort2),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority3Port",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3003),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort3),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("Priority4Port",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3004),
                   MakeUintegerAccessor (&StrictPriorityQueue::m_priorityPort4),
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
      priority = 0;
  else if(q_class[1]->filters[0]->element[0]->match(p))
      priority = 1;
  else if(q_class[2]->filters[0]->element[0]->match(p))
      priority = 2;
  else if(q_class[3]->filters[0]->element[0]->match(p))
      priority = 3;
  else
      priority = 4; 

  return priority;
}

bool
StrictPriorityQueue::DoEnqueue (Ptr<QueueItem> p)
{
  NS_LOG_FUNCTION (this << p);

  uint16_t priority = classify (p);

  // Higher Priority
  if (priority == 0)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[0]->m_queue.size() >= q_class[0]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[0]->getBytes() + p->GetPacket()->GetSize() >= q_class[0]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      q_class[0]->setBytes(q_class[0]->getBytes() + p->GetPacket()->GetSize());
      q_class[0]->m_queue.push(p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[0]->m_queue.size());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[0]->getBytes());
      
      return true; 
    }

  // Low Priority
  else if (priority == 1)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[1]->m_queue.size() >= q_class[1]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[1]->getBytes() + p->GetPacket()->GetSize() >= q_class[1]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      q_class[1]->setBytes( q_class[1]->getBytes() + p->GetPacket()->GetSize ());
      q_class[1]->m_queue.push (p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[1]->m_queue.size ());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[1]->getBytes());
      
      return true;      
    }

    // Low Priority
  else if (priority == 2)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[2]->m_queue.size() >= q_class[2]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[2]->getBytes() + p->GetPacket()->GetSize() >= q_class[2]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      q_class[2]->setBytes( q_class[2]->getBytes() + p->GetPacket()->GetSize ());
      q_class[2]->m_queue.push (p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[2]->m_queue.size ());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[2]->getBytes());
      
      return true;      
    }
    // Low Priority
  else if (priority == 3)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[3]->m_queue.size() >= q_class[3]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[3]->getBytes() + p->GetPacket()->GetSize() >= q_class[3]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      q_class[3]->setBytes( q_class[3]->getBytes() + p->GetPacket()->GetSize ());
      q_class[3]->m_queue.push (p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[3]->m_queue.size ());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[3]->getBytes());
      
      return true;      
    }
    else if (priority == 4)
    {
      if (m_mode == QUEUE_MODE_PACKETS && (q_class[4]->m_queue.size() >= q_class[4]->getMaxPackets()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (at max packets) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      if (m_mode == QUEUE_MODE_BYTES && (q_class[4]->getBytes() + p->GetPacket()->GetSize() >= q_class[4]->getMaxBytes()))
        {
          NS_LOG_LOGIC ("SPQ: Queue full (packet would exceed max bytes) -- droppping pkt");
          Drop (p->GetPacket());
          return false;
        }
      
      q_class[4]->setBytes( q_class[4]->getBytes() + p->GetPacket()->GetSize ());
      q_class[4]->m_queue.push (p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[4]->m_queue.size ());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[4]->getBytes());
      
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
      
      NS_LOG_LOGIC ("SPQ: Popped " << p);
      
      NS_LOG_LOGIC ("SPQ: Number packets " << q_class[0]->m_queue.size());
      NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[0]->getBytes());
      
      return new QueueItem(p->GetPacket());
    }
  else if(!q_class[1]->m_queue.empty ())
    { 
      NS_LOG_LOGIC ("SPQ: Highest priority queue empty, serving lower priority queue");

      if (!q_class[1]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[1]->m_queue.front ();
          q_class[1]->m_queue.pop ();
          q_class[1]->setBytes( (q_class[1]->getBytes()) - (p->GetPacket()->GetSize()));
          
          NS_LOG_LOGIC ("SPQ: Popped " << p);
          
          NS_LOG_LOGIC ("SPQ: Number packets " << q_class[1]->m_queue.size());
          NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[1]->getBytes());
          
          return  new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("SPQ: Lower priority queue empty");
          return 0;
        } 
    } 
      else if(!q_class[2]->m_queue.empty ())
    { 
      NS_LOG_LOGIC ("SPQ: Higher priority queue empty, serving lower priority queue");

      if (!q_class[2]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[2]->m_queue.front ();
          q_class[2]->m_queue.pop ();
          q_class[2]->setBytes( (q_class[2]->getBytes()) - (p->GetPacket()->GetSize()));
          
          NS_LOG_LOGIC ("SPQ: Popped " << p);
          
          NS_LOG_LOGIC ("SPQ: Number packets " << q_class[2]->m_queue.size());
          NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[2]->getBytes());
          
          return  new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("SPQ: Lower priority queue empty");
          return 0;
        } 
    }
    else if(!q_class[3]->m_queue.empty ())
    { 
      NS_LOG_LOGIC ("SPQ: Higher priority queue empty, serving lower priority queue");

      if (!q_class[3]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[3]->m_queue.front ();
          q_class[3]->m_queue.pop ();
          q_class[3]->setBytes( (q_class[3]->getBytes()) - (p->GetPacket()->GetSize()));
          
          NS_LOG_LOGIC ("SPQ: Popped " << p);
          
          NS_LOG_LOGIC ("SPQ: Number packets " << q_class[3]->m_queue.size());
          NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[3]->getBytes());
          
          return  new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("SPQ: Lower priority queue empty");
          return 0;
        } 
    }
    else 
    { 
      NS_LOG_LOGIC ("SPQ: Higher priority queue empty, serving lowest priority queue");

      if (!q_class[4]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[4]->m_queue.front ();
          q_class[4]->m_queue.pop ();
          q_class[4]->setBytes( (q_class[4]->getBytes()) - (p->GetPacket()->GetSize()));
          
          NS_LOG_LOGIC ("SPQ: Popped " << p);
          
          NS_LOG_LOGIC ("SPQ: Number packets " << q_class[4]->m_queue.size());
          NS_LOG_LOGIC ("SPQ: Number bytes " << q_class[4]->getBytes());
          
          return  new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("SPQ: Lowest priority queue empty");
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
      
      NS_LOG_LOGIC ("Number packets " << q_class[0]->m_queue.size());
      NS_LOG_LOGIC ("Number bytes " << q_class[0]->getBytes());
      
      return new QueueItem(p->GetPacket());
    }
  else
    {
      NS_LOG_LOGIC ("High priority queue empty");

      if (!q_class[1]->m_queue.empty ())
        {
          Ptr<QueueItem> p = q_class[1]->m_queue.front ();
          
          NS_LOG_LOGIC ("Number packets " << q_class[1]->m_queue.size());
          NS_LOG_LOGIC ("Number bytes " << q_class[1]->getBytes());
          
          return new QueueItem(p->GetPacket());
        }
      else
        {
          NS_LOG_LOGIC ("Low priority queue empty");
          return 0;
        }
    }
}

} // namespace ns3
