/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include "base-class.h"

namespace ns3 {

BaseClass::BaseClass(){
}

BaseClass::~BaseClass(){
}

void BaseClass::setMode(BaseClass::QueueMode mode){

        m_mode = mode;

}


BaseClass::QueueMode BaseClass::getMode(void){
		return m_mode;
}


bool BaseClass:: Enqueue(Ptr<ns3::Packet> p){
      return DoEnqueue(p);
}

Ptr<ns3::Packet> BaseClass:: Dequeue(void){
        return DoDequeue();
}


bool BaseClass:: DoEnqueue(Ptr<ns3::Packet> p){

        return true;
}


Ptr<ns3::Packet> BaseClass:: DoDequeue(void){

        return 0;

}




}

