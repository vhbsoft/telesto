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

NS_LOG_COMPONENT_DEFINE("DeficitRoundRobin");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(DeficitRoundRobin);

TypeId DeficitRoundRobin::GetTypeId(void) {
	static TypeId tid = TypeId("ns3::DeficitRoundRobin")
     .SetParent<BaseClass> ()
     .AddConstructor<DeficitRoundRobin> ()
     .AddAttribute ("DRRMode",
                    "Whether to use bytes (see MaxBytes) or packets (see MaxPackets) as the maximum queue size metric.",
                   EnumValue (QUEUE_MODE_PACKETS),
                   MakeEnumAccessor (&DeficitRoundRobin::SetMode),
                   MakeEnumChecker (QUEUE_MODE_BYTES, "QUEUE_MODE_BYTES",
                                    QUEUE_MODE_PACKETS, "QUEUE_MODE_PACKETS"))
    .AddAttribute ("firstMaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_firstMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("secondMaxPackets",
                   "The maximum number of packets accepted by the low priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_secondMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("thirdMaxPackets",
                   "The maximum number of packets accepted by the high priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_thirdMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("fourthMaxPackets",
                   "The maximum number of packets accepted by the low priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_fourthMaxPackets),
                   MakeUintegerChecker<uint32_t> ())

    .AddAttribute ("fifthMaxPackets",
                   "The maximum number of packets accepted by the low priority queue.",
                   UintegerValue (100),
                   MakeUintegerAccessor (&DeficitRoundRobin::m_fifthMaxPackets),
                   MakeUintegerChecker<uint32_t> ())    


      .AddAttribute("QuantumSize1",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize1),
                  MakeUintegerChecker<uint32_t>())

      .AddAttribute("QuantumSize2",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize2),
                  MakeUintegerChecker<uint32_t>())

      .AddAttribute("QuantumSize3",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize3),
                  MakeUintegerChecker<uint32_t>())                        

      .AddAttribute("QuantumSize4",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize4),
                  MakeUintegerChecker<uint32_t>())

      .AddAttribute("QuantumSize5",
                  "The packet size permitted for each queue",
                  UintegerValue(1),
                  MakeUintegerAccessor(&DeficitRoundRobin::quantumSize5),
                  MakeUintegerChecker<uint32_t>());
	return tid;
}

DeficitRoundRobin::DeficitRoundRobin(): BaseClass(){
  this->m_serve_queue1 = true;
  this->m_serve_queue2 = false;
  this->m_serve_queue3 = false;
  this->m_serve_queue4 = false;
  this->m_serve_queue5 = false;
  this->m_firstMaxPackets = 100;
  this->m_secondMaxPackets = 100;
  this->m_thirdMaxPackets = 100;
  this->m_fourthMaxPackets = 100;
  this->m_fifthMaxPackets = 100;   

  this->m_firstMaxBytes = 100*65535;
  this->m_secondMaxBytes = 100*65535;
  this->m_thirdMaxBytes = 100*65535;
  this->m_fourthMaxBytes = 100*65535; 
  this->m_fifthMaxBytes = 100*65535;   
  this->m_firstPort = 3000;
  this->m_secondPort = 3001;
  this->m_thirdPort = 3002;
  this->m_fourthPort = 3003;
  this->m_fifthPort = 3004;


  q_class[0] = new TrafficClass;              //initialize 1 priority queue
  q_class[1] = new TrafficClass;              //initialize 2  priority queue
  q_class[2] = new TrafficClass;              //initialize 3 priority queue
  q_class[3] = new TrafficClass;              //initialize 4  priority queue
  q_class[4] = new TrafficClass;              //initialize 5 priority queue   
  q_class[0]->setBytes(0);                    //initialize
  q_class[1]->setBytes(0);                    //initialize
  q_class[2]->setBytes(0);                    //initialize
  q_class[3]->setBytes(0);                    //initialize
  q_class[4]->setBytes(0);                    //initialize    
  q_class[0]->setMaxPackets(m_firstMaxPackets);//Custom settings
  q_class[1]->setMaxPackets(m_secondMaxPackets); //Custom settings
  q_class[2]->setMaxPackets(m_thirdMaxPackets);//Custom settings
  q_class[3]->setMaxPackets(m_fourthMaxPackets); //Custom settings
  q_class[4]->setMaxPackets(m_fifthMaxPackets); //Custom settings     
  q_class[0]->setMaxBytes(m_firstMaxBytes);    //Custom settings
  q_class[1]->setMaxBytes(m_secondMaxBytes);     //Custom settings
  q_class[2]->setMaxBytes(m_thirdMaxBytes);    //Custom settings
  q_class[3]->setMaxBytes(m_fourthMaxBytes);     //Custom settings
  q_class[4]->setMaxBytes(m_fifthMaxBytes);    //Custom settings   
  DestinationPortNumber * PN1 = new DestinationPortNumber(m_firstPort);
  DestinationPortNumber * PN2 = new DestinationPortNumber(m_secondPort);
  DestinationPortNumber * PN3 = new DestinationPortNumber(m_thirdPort);
  DestinationPortNumber * PN4 = new DestinationPortNumber(m_fourthPort);
  DestinationPortNumber * PN5 = new DestinationPortNumber(m_fifthPort); 

  Filter * fil1;
  fil1->set_element(PN1);
  q_class[0]->filters.push_back(fil1);

  Filter * fil2;
  fil2->set_element(PN2);
  q_class[1]->filters.push_back(fil2);

  Filter * fil3;
  fil3->set_element(PN3);
  q_class[2]->filters.push_back(fil3);

  Filter * fil4;
  fil4->set_element(PN4);
  q_class[3]->filters.push_back(fil4);

  Filter * fil5;
  fil5->set_element(PN5);
  q_class[4]->filters.push_back(fil5);

  NS_LOG_FUNCTION_NOARGS ();
}

DeficitRoundRobin::~DeficitRoundRobin() {
	NS_LOG_FUNCTION_NOARGS ();
}


void
DeficitRoundRobin::SetMode (DeficitRoundRobin::QueueMode mode)
{
  NS_LOG_FUNCTION (mode);
  m_mode = mode;
}

DeficitRoundRobin::QueueMode
DeficitRoundRobin::GetMode (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  return m_mode;
}



uint16_t DeficitRoundRobin::classify(Ptr<QueueItem> p) {

   uint16_t weightedQueue;
   if(q_class[0]->filters[0]->element[0]->match(p))
      weightedQueue = 0;
   else if(q_class[1]->filters[0]->element[0]->match(p))
     weightedQueue = 1;
   else if(q_class[2]->filters[0]->element[0]->match(p))
     weightedQueue = 2;
   else if(q_class[3]->filters[0]->element[0]->match(p))
     weightedQueue = 3;
   else
     weightedQueue = 4; 

	return weightedQueue;
}



bool DeficitRoundRobin::DoEnqueue(Ptr<QueueItem> p) {
	NS_LOG_FUNCTION(this << p);

	uint16_t weightedQueue = classify(p);

	// Second Queue
	if (weightedQueue == 1) {
		if (m_mode == QUEUE_MODE_PACKETS
				&& (q_class[0]->m_queue.size() >= q_class[0]->getMaxPackets())) {
			NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
			Drop(p->GetPacket());
			return false;
		}

		if (m_mode == QUEUE_MODE_BYTES
				&& (q_class[0]->getBytes() + p->GetPacket()->GetSize() >= q_class[0]->getMaxBytes())) {
			NS_LOG_LOGIC(
					"Queue full (packet would exceed max bytes) -- dropping pkt");
			Drop(p->GetPacket());
			return false;
		}

		q_class[0]->setBytes(q_class[0]->getBytes() + p->GetPacket()->GetSize());
		//q_class[0]->m_queue.push(p);


		return true;
	}

	// First Queue
	else if (weightedQueue == 0) {
		if (m_mode == QUEUE_MODE_PACKETS
				&& (q_class[1]->m_queue.size() >= q_class[1]->getMaxPackets())) {
			NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
			Drop(p->GetPacket());
			return false;
		}

		if (m_mode == QUEUE_MODE_BYTES
				&& (q_class[1]->getBytes() + p->GetPacket()->GetSize() >= q_class[1]->getMaxBytes())) {
			NS_LOG_LOGIC(
					"Queue full (packet would exceed max bytes) -- dropping pkt");
			Drop(p->GetPacket());
			return false;
		}

    q_class[1]->setBytes(q_class[1]->getBytes() + p->GetPacket()->GetSize());
		//q_class[1]->m_queue.push(p);

		return true;
	}

  else if (weightedQueue == 2) {
    if (m_mode == QUEUE_MODE_PACKETS
        && (q_class[2]->m_queue.size() >= q_class[2]->getMaxPackets())) {
      NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    if (m_mode == QUEUE_MODE_BYTES
        && (q_class[2]->getBytes() + p->GetPacket()->GetSize() >= q_class[2]->getMaxBytes())) {
      NS_LOG_LOGIC(
          "Queue full (packet would exceed max bytes) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    q_class[2]->setBytes(q_class[2]->getBytes() + p->GetPacket()->GetSize());
    //q_class[2]->m_queue.push(p);

    return true;
  }  

    else if (weightedQueue == 3) {
    if (m_mode == QUEUE_MODE_PACKETS
        && (q_class[3]->m_queue.size() >= q_class[3]->getMaxPackets())) {
      NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    if (m_mode == QUEUE_MODE_BYTES
        && (q_class[3]->getBytes() + p->GetPacket()->GetSize() >= q_class[3]->getMaxBytes())) {
      NS_LOG_LOGIC(
          "Queue full (packet would exceed max bytes) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    q_class[3]->setBytes(q_class[3]->getBytes() + p->GetPacket()->GetSize());
    //q_class[3]->m_queue.push(p);

    return true;
  }

  else if (weightedQueue == 4) {
    if (m_mode == QUEUE_MODE_PACKETS
        && (q_class[4]->m_queue.size() >= q_class[4]->getMaxPackets())) {
      NS_LOG_LOGIC("Queue full (at max packets) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    if (m_mode == QUEUE_MODE_BYTES
        && (q_class[4]->getBytes() + p->GetPacket()->GetSize() >= q_class[4]->getMaxBytes())) {
      NS_LOG_LOGIC(
          "Queue full (packet would exceed max bytes) -- dropping pkt");
      Drop(p->GetPacket());
      return false;
    }

    q_class[4]->setBytes(q_class[4]->getBytes() + p->GetPacket()->GetSize());
    q_class[4]->m_queue.push(p);

    return true;
  }  

	// This normally never happens unless Classify() has been changed
	else {
		return false;
	}
}



Ptr<QueueItem> DeficitRoundRobin::DoDequeue(void) {
	NS_LOG_FUNCTION(this);

    if(!q_class[1]->m_queue.empty() || !q_class[0]->m_queue.empty()){
        if (!q_class[1]->m_queue.empty() && m_serve_queue1) { //Check == false
                m_first_dc += quantumSize2;
                Ptr<QueueItem> packet1 = q_class[1]->m_queue.front();
                NS_LOG_LOGIC("-------------------Queue1 is being served-----------------------");
                NS_LOG_LOGIC("Number bytes in first queue1 " << q_class[1]->getBytes());
                NS_LOG_LOGIC("Defecit Counter Queue1 before " << m_first_dc);
               if(packet1->GetPacket()->GetSize() < m_first_dc){
                        q_class[1]->setBytes(q_class[1]->getBytes() - packet1->GetPacket()->GetSize());
                        //q_class[1]->m_queue.pop();
                        m_first_dc -= packet1->GetPacket()->GetSize();
                        
                }
                
                if(q_class[1]->m_queue.empty()){
                	m_first_dc = 0;
                }
    
                
                NS_LOG_LOGIC("Number packets " << q_class[1]->m_queue.size ());
                NS_LOG_LOGIC("Packet SIze " << packet1->GetPacket()->GetSize());
                NS_LOG_LOGIC("Number bytes in first queue2 " << q_class[1]->getBytes());
                
                
                m_serve_queue1 = false;
                m_serve_queue2 = true;
                NS_LOG_LOGIC("Defecit Counter Queue1 After " << m_first_dc);
                return new QueueItem(packet1->GetPacket());
        }

        else if(!q_class[0]->m_queue.empty() && m_serve_queue2) { //check == true
                m_second_dc += quantumSize1;
                Ptr<QueueItem> packet2 = q_class[0]->m_queue.front();
                NS_LOG_LOGIC("------------------Queue2 is being served-------------------------");                
                NS_LOG_LOGIC("Defecit Counter Queue2 before" << m_second_dc);                
                if(packet2->GetPacket()->GetSize() < m_second_dc){
                        q_class[0]->setBytes(q_class[0]->getBytes() - packet2->GetPacket()->GetSize());
                        //q_class[0]->m_queue.pop();
                        m_second_dc -= packet2->GetPacket()->GetSize();
                        
                }

                
                if(q_class[0]->m_queue.empty()){
                  m_second_dc = 0;
                }

                NS_LOG_LOGIC("Number packets " << q_class[0]->m_queue.size ());
                NS_LOG_LOGIC("Packet SIZE " << packet2->GetPacket()->GetSize());
                NS_LOG_LOGIC("Number bytes in second Queue " << q_class[0]->getBytes());
                NS_LOG_LOGIC("HELLOOOO");              

                m_serve_queue2=false;
                m_serve_queue3=true;

                NS_LOG_LOGIC("Defecit Counter Queue2 After" << m_second_dc);
                return new QueueItem(packet2->GetPacket());
        }
        else if(!q_class[2]->m_queue.empty() && m_serve_queue3) { //check == true
                m_third_dc += quantumSize3;
                Ptr<QueueItem> packet3 = q_class[2]->m_queue.front();
                NS_LOG_LOGIC("------------------Queue2 is being served-------------------------");                
                NS_LOG_LOGIC("Defecit Counter Queue2 before" << m_third_dc);                
                if(packet3->GetPacket()->GetSize() < m_third_dc){
                        q_class[2]->setBytes(q_class[2]->getBytes() - packet3->GetPacket()->GetSize());
                        //q_class[2]->m_queue.pop();
                        m_third_dc -= packet3->GetPacket()->GetSize();
                        
                }

                
                if(q_class[2]->m_queue.empty()){
                  m_third_dc = 0;
                }

                NS_LOG_LOGIC("Number packets " << q_class[2]->m_queue.size ());
                NS_LOG_LOGIC("Packet SIZE " << packet3->GetPacket()->GetSize()); 
                NS_LOG_LOGIC("Number bytes in second Queue " << q_class[2]->getBytes());
                               

                m_serve_queue3=false;
                m_serve_queue4=true;
                NS_LOG_LOGIC("Defecit Counter Queue2 After" << m_third_dc);
                return new QueueItem(packet3->GetPacket());
        } 

        else if(!q_class[3]->m_queue.empty() && m_serve_queue4) { //check == true
                m_fourth_dc += quantumSize4;
                Ptr<QueueItem> packet4 = q_class[3]->m_queue.front();
                NS_LOG_LOGIC("------------------Queue2 is being served-------------------------");                
                NS_LOG_LOGIC("Defecit Counter Queue2 before" << m_fourth_dc);                
                if(packet4->GetPacket()->GetSize() < m_fourth_dc){
                        q_class[3]->setBytes(q_class[3]->getBytes() - packet4->GetPacket()->GetSize());
                        //q_class[3]->m_queue.pop();
                        m_fourth_dc -= packet4->GetPacket()->GetSize();
                        
                }

                
                if(q_class[3]->m_queue.empty()){
                  m_fourth_dc = 0;
                }

                NS_LOG_LOGIC("Number packets " << q_class[3]->m_queue.size ());
                NS_LOG_LOGIC("Packet SIZE " << packet4->GetPacket()->GetSize()); 
                NS_LOG_LOGIC("Number bytes in second Queue " << q_class[3]->getBytes());
                               

                m_serve_queue4=false;
                m_serve_queue5=true;
                NS_LOG_LOGIC("Defecit Counter Queue2 After" << m_fourth_dc);
                return new QueueItem(packet4->GetPacket());
        }    

        else if(!q_class[4]->m_queue.empty() && m_serve_queue5) { //check == true
                m_fifth_dc += quantumSize5;
                Ptr<QueueItem> packet5 = q_class[4]->m_queue.front();
                NS_LOG_LOGIC("------------------Queue2 is being served-------------------------");                
                NS_LOG_LOGIC("Defecit Counter Queue2 before" << m_fifth_dc);                
                if(packet5->GetPacket()->GetSize() < m_fifth_dc){
                        q_class[4]->setBytes(q_class[4]->getBytes() - packet5->GetPacket()->GetSize());
                        //q_class[4]->m_queue.pop();
                        m_fifth_dc -= packet5->GetPacket()->GetSize();
                        
                }

                
                if(q_class[4]->m_queue.empty()){
                  m_fifth_dc = 0;
                }

                NS_LOG_LOGIC("Number packets " << q_class[4]->m_queue.size ());
                NS_LOG_LOGIC("Packet SIZE " << packet5->GetPacket()->GetSize()); 
                NS_LOG_LOGIC("Number bytes in second Queue " << q_class[4]->getBytes());
                               

                m_serve_queue5=false;
                m_serve_queue1=true;
                NS_LOG_LOGIC("Defecit Counter Queue2 After" << m_second_dc);
                return new QueueItem(packet5->GetPacket());
        }  



        else
                return NULL;
    }

    else{

		NS_LOG_LOGIC("all queues empty");
		return NULL;
    }
}



Ptr<const QueueItem> DeficitRoundRobin::DoPeek(void) const {
	NS_LOG_FUNCTION(this);

	int32_t queueToBeServed = GetQueueToBePeeked();

	if (queueToBeServed == 0) {

		Ptr<Packet> pack1 = q_class[1]->m_queue.front()->GetPacket();
		NS_LOG_LOGIC("Number packets " << q_class[1]->m_queue.size ());
		NS_LOG_LOGIC("Number bytes " << q_class[1]->getBytes());
                
		return new QueueItem(pack1);
	}

	else if (queueToBeServed == 1) {

		Ptr<Packet> pack2 = q_class[0]->m_queue.front()->GetPacket();
		NS_LOG_LOGIC("Number packets " << q_class[0]->m_queue.size ());
		NS_LOG_LOGIC("Number bytes " << q_class[1]->getBytes());

		return new QueueItem(pack2);
	}

	else {
		NS_LOG_LOGIC("all queues empty");
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


void DeficitRoundRobin::schedule(Ptr<QueueItem> p){

  return;
}

} // namespace ns3
