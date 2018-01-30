/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef BASE_CLASS_H
#define BASE_CLASS_H

#include <queue>
#include "ns3/packet.h"
#include "ns3/queue.h"
#include "ns3/nstime.h"
#include <vector>
#include "traffic-class.h"


namespace ns3 {

class BaseClass: public Queue<ns3::Packet>{
        protected:
                BaseClass::QueueMode m_mode;
                std::vector<TrafficClass*> q_class;
                virtual bool Enqueue(Ptr<ns3::Packet> p);
                virtual Ptr<ns3::Packet> Dequeue(void);
                virtual bool DoEnqueue(Ptr<ns3::Packet> p);
                virtual Ptr<ns3::Packet> DoDequeue(void);
                virtual void schedule(Ptr<ns3::Packet> p)=0;
                virtual uint16_t classify(Ptr<ns3::Packet> p)=0;

        public:
                BaseClass();
                ~BaseClass();
                void setMode(BaseClass::QueueMode mode);
                BaseClass::QueueMode getMode(void);
                



};

}

#endif /* BASE_CLASS_H */

