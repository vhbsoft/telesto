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

class BaseClass: public QueueBase{
        protected:
                BaseClass::QueueMode m_mode;
                std::vector<TrafficClass*> q_class;
                virtual bool Enqueue(Ptr<QueueItem> p);
                virtual Ptr<QueueItem> Dequeue(void);
                virtual bool DoEnqueue(Ptr<QueueItem> p);
                virtual Ptr<QueueItem> DoDequeue(void);
                virtual void schedule(Ptr<QueueItem> p)=0;
                virtual uint16_t classify(Ptr<QueueItem> p)=0;

        public:
                BaseClass();
                ~BaseClass();
                void setMode(BaseClass::QueueMode mode);
                BaseClass::QueueMode getMode(void);
                



};

}

#endif /* BASE_CLASS_H */

