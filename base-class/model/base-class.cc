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


bool BaseClass:: Enqueue(Ptr<QueueItem> p){
      return DoEnqueue(p);
}

Ptr<QueueItem> BaseClass:: Dequeue(void){
        return DoDequeue();
}


bool BaseClass:: DoEnqueue(Ptr<QueueItem> p){

        return true;
}


Ptr<QueueItem> BaseClass:: DoDequeue(void){

        return 0;

}




}

