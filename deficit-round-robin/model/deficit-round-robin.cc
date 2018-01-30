/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "ns3/log.h"
#include "ns3/enum.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "deficit-round-robin.h"

#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/simulator.h"

#include "ns3/ppp-header.h"
#include "ns3/udp-header.h"
#include "ns3/tcp-header.h"
#include "ns3/ipv4-header.h"

#include <math.h>

//NS_LOG_COMPONENT_DEFINE("DeficitRoundRobin");

namespace ns3 {

//NS_OBJECT_ENSURE_REGISTERED(DeficitRoundRobin);

TypeId DeficitRoundRobin::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::DeficitRoundRobin")
     .SetParent<BaseClass> ()
     .AddConstructor<DeficitRoundRobin> ()
     .AddAttribute ("Mode",
                    "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_MODE_PACKETS),
                   MakeEnumAccessor (&DeficitRoundRobin::SetMode),
                   MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES",
                                    QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    .AddAttribute ("HighPriorityMaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_highMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("LowPriorityMaxPackets",
                   "The maximum number of packets accepted by the low priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_lowMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("HighPriorityMaxBytes",
                   "The maximum number of bytes accepted by the high priority queue.",
                   UintegerValue (100 * 65535),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_highMaxBytes),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("LowPriorityMaxBytes",
                   "The maximum number of bytes accepted by the low priority queue.",
                   UintegerValue (100 * 65535),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_lowMaxBytes),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("HighPriorityPort",
                   "The destination port number for high priority traffic.",
                   UintegerValue (3000),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_priorityPort),
                   MakeUintegerChecker<uint32_t> ())
      .AddAttribute("QuantumSize",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize),
                  MakeUintegerChecker<uint32_t>());
	return tid;
}

DeficitRoundRobin::DeficitRoundRobin(): BaseClass(){
  this->m_serve_queue2 = false;
  this->m_highMaxPackets = 100;
  this->m_lowMaxPackets = 100;
  this->m_highMaxBytes = 100*65535;
  this->m_lowMaxBytes = 100*65535;
  this->m_priorityPort = 3000;


  q_class[0] = new TrafficClass;              //initialize high priority queue
  q_class[1] = new TrafficClass;              //initialize low  priority queue
  q_class[0]->setBytes(0);                    //initialize
  q_class[1]->setBytes(0);                    //initialize
  q_class[0]->setMaxPackets(m_highMaxPackets);//Custom settings
  q_class[1]->setMaxPackets(m_lowMaxPackets); //Custom settings
  q_class[0]->setMaxBytes(m_highMaxBytes);    //Custom settings
  q_class[1]->setMaxBytes(m_lowMaxBytes);     //Custom settings
  DestinationPortNumber * prioPN = new DestinationPortNumber(m_priorityPort); 

  Filter * fil;
  fil->set_element(prioPN);
  q_class[0]->filters.push_back(fil);


  //NS_LOG_FUNCTION_NOARGS ();
}

DeficitRoundRobin::~DeficitRoundRobin() {
	//NS_LOG_FUNCTION_NOARGS ();
}


void
DeficitRoundRobin::SetMode (DeficitRoundRobin::QueueMode mode)
{
  //NS_LOG_FUNCTION (mode);
  m_mode = mode;
}

DeficitRoundRobin::QueueMode
DeficitRoundRobin::GetMode (void)
{
  //NS_LOG_FUNCTION_NOARGS ();
  return m_mode;
}



uint16_t DeficitRoundRobin::classify(Ptr<ns3::Packet> p) {

   uint16_t weightedQueue;
   if(q_class[0]->filters[0]->element[0]->match(p))
      weightedQueue = 1;
    else 
     weightedQueue = 0;

	return weightedQueue;
}



bool DeficitRoundRobin::DoEnqueue(Ptr<ns3::Packet> p) {
	//NS_LOG_FUNCTION(this << p);

	uint16_t weightedQueue = classify(p);

	// Second Queue
	if (weightedQueue == 1) {
		if (m_mode == QUEUE_MODE_PACKETS
				&& (q_class[0]->m_queue.size() >= q_class[0]->getMaxPackets())) {
			//NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
			DoRemove(p);
			return false;
		}

		if (m_mode == QUEUE_MODE_BYTES
				&& (q_class[0]->getBytes() + p->GetSize() >= q_class[0]->getMaxBytes())) {
			//NS_LOG_LOGIC("Queue full (packet would exceed max bytes) -- dropping pkt");
			DoRemove(p);
			return false;
		}

		q_class[0]->setBytes(q_class[0]->getBytes() + p->GetSize());
		q_class[0]->m_queue.push(p);


		return true;
	}

	// First Queue
	else if (weightedQueue == 0) {
		if (m_mode == QUEUE_MODE_PACKETS
				&& (q_class[1]->m_queue.size() >= q_class[1]->getMaxPackets())) {
			//NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
			DoRemove(p);
			return false;
		}

		if (m_mode == QUEUE_MODE_BYTES
				&& (q_class[1]->getBytes() + p->GetPacket()->GetSize() >= q_class[1]->getMaxBytes())) {
			//NS_LOG_LOGIC("Queue full (packet would exceed max bytes) -- dropping pkt");
			DoRemove(p);
			return false;
		}

    q_class[1]->setBytes(q_class[1]->getBytes() + p->GetSize());
		q_class[1]->m_queue.push(p);

		////NS_LOG_LOGIC("Number packets " << m_firstQueue.size ());
		////NS_LOG_LOGIC("Number bytes " << m_bytesInFirstQueue);

		return true;
	}

	// This normally never happens unless Classify() has been changed
	else {
		return false;
	}
}



Ptr<ns3::Packet> DeficitRoundRobin::DoDequeue(void) {
	//NS_LOG_FUNCTION(this);

    if(!q_class[1]->m_queue.empty() || !q_class[0]->m_queue.empty()){
        if (!q_class[1]->m_queue.empty() && !m_serve_queue2) { //Check == false
                m_first_dc += quantumSize;
                Ptr<ns3::Packet> packet1 = q_class[1]->m_queue.front();
                //NS_LOG_LOGIC("-------------------Queue1 is being served-----------------------");
                //NS_LOG_LOGIC("Number bytes in first queue1 " << q_class[1]->getBytes());
                //NS_LOG_LOGIC("Defecit Counter Queue1 before " << m_first_dc);
               if(packet1->GetSize() < m_first_dc){
                        q_class[1]->setBytes(q_class[1]->getBytes() - packet1->GetSize());
                        q_class[1]->m_queue.pop();
                        m_first_dc -= packet1->GetSize();
                        
                }
                
                if(q_class[1]->m_queue.empty()){
                	m_first_dc = 0;
                }
    
                
                //NS_LOG_LOGIC("Number packets " << q_class[1]->m_queue.size ());
                //NS_LOG_LOGIC("Packet SIze " << packet1->GetPacket()->GetSize());
                //NS_LOG_LOGIC("Number bytes in first queue2 " << q_class[1]->getBytes());
                
                

                m_serve_queue2 = true;
                //NS_LOG_LOGIC("Defecit Counter Queue1 After " << m_first_dc);
                return new ns3::Packet(packet1->GetPacket());
        }

        else if(!q_class[0]->m_queue.empty() && m_serve_queue2) { //check == true
                m_second_dc += quantumSize;
                Ptr<ns3::Packet> packet2 = q_class[0]->m_queue.front();
                //NS_LOG_LOGIC("------------------Queue2 is being served-------------------------");                
                //NS_LOG_LOGIC("Defecit Counter Queue2 before" << m_second_dc);                
                if(packet2->GetSize() < m_second_dc){
                        q_class[0]->setBytes(q_class[0]->getBytes() - packet2->GetSize());
                        q_class[0]->m_queue.pop();
                        m_second_dc -= packet2->GetSize();
                        
                }

                
                //if(m_secondQueue.empty()){
                  //      m_second_dc = 0;
                //}

                //NS_LOG_LOGIC("Number packets " << q_class[0]->m_queue.size ());
                //NS_LOG_LOGIC("Packet SIZE " << packet2->GetPacket()->GetSize()); 
                //NS_LOG_LOGIC("Number bytes in second Queue " << q_class[0]->getBytes());
                               

                m_serve_queue2=false;
                //NS_LOG_LOGIC("Defecit Counter Queue2 After" << m_second_dc);
                return new ns3::Packet(packet2);
        }
        else
                return NULL;
    }

    else{

		//NS_LOG_LOGIC("all queues empty");
		return NULL;
    }
}



Ptr<const ns3::Packet> DeficitRoundRobin::DoPeek(void) const {
	//NS_LOG_FUNCTION(this);

	int32_t queueToBeServed = GetQueueToBePeeked();

	if (queueToBeServed == 0) {

		Ptr<Packet> pack1 = q_class[1]->m_queue.front()->GetPacket();
		//NS_LOG_LOGIC("Number packets " << q_class[1]->m_queue.size ());
		//NS_LOG_LOGIC("Number bytes " << q_class[1]->getBytes());
                
		return new ns3::Packet(pack1);
	}

	else if (queueToBeServed == 1) {

		Ptr<Packet> pack2 = q_class[0]->m_queue.front()->GetPacket();
		//NS_LOG_LOGIC("Number packets " << q_class[0]->m_queue.size ());
		//NS_LOG_LOGIC("Number bytes " << q_class[1]->getBytes());

		return new ns3::Packet(pack2);
	}

	else {
		//NS_LOG_LOGIC("all queues empty");
		return 0;
	}
}


int32_t DeficitRoundRobin::GetQueueToBePeeked() const
{
	if (q_class[1]->getPackets() > 0)
	{
		if (!q_class[1]->m_queue.empty())
		{
			return 0;
		}
	}

	if (q_class[0]->getPackets() > 0)
	{
		if (!q_class[0]->m_queue.empty())
		{
			return 1;
		}
	}

	if (!q_class[1]->m_queue.empty())
	{
		return 0;
	}

	if (!q_class[0]->m_queue.empty())
	{
		return 1;
	}

	return -1;
}


void DeficitRoundRobin::schedule(Ptr<ns3::Packet> p){

  return;
}

} // namespace ns3
