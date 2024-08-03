/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// Network topology
//       S1 ------|        |-------R1
//                |        |
//                N1 ---- N2
//                |        |
//       S2-------|        |-------R2
//
// - Flow from S1 to R1 and S2 to R2 using FTP (BulkSendApplication).
// - DRR implemented on N1
// ###############################################################
// #    EXECUTE THIS TEST CODE FOR VERIFICATION!
// #    run this on cmd = ./waf --run "drr-validation -quantumSize=321"
// #    -quantumSize = [value you want]
// ###############################################################
#include <string>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/network-module.h"
#include "ns3/custom-packet-sink.h"
#include "ns3/deficit-round-robin.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/base-class.h"
#include <sstream>
#include <math.h>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("DRR_Validation");

Ptr<PointToPointNetDevice> NetDeviceDynamicCast (Ptr<NetDevice> const&p)
{
  return Ptr<PointToPointNetDevice> (dynamic_cast<PointToPointNetDevice *> (PeekPointer (p)));
}

int
main (int argc, char *argv[])
{
  // Set the TCP variant to CUBIC
  //Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpCubic::GetTypeId()));

  // Set the TCP variant to NewReno (It's not mandatory since NewReno is NS3 Default)
  //Config::SetDefault ("ns3::TcpL4Protocol::SocketType", TypeIdValue (TcpNewReno::GetTypeId()));


// ############################
// Logging for Verification
  LogComponentEnable ("DeficitRoundRobin", LOG_LEVEL_LOGIC);
// #############################

  bool tracing = false;
  uint16_t secondQueuePort = 3001;
  uint16_t R1port = 3000;
  uint16_t R2port = 3001;
  uint16_t R3port = 3002;
  uint16_t R4port = 3003;
  uint16_t R5port = 3004;
  uint32_t tcp_adu_size = 2300;  //946

  uint32_t DEFAULT_DATA_BYTES = 1073741824; //1000000000;
  uint32_t ftp1_data_bytes = DEFAULT_DATA_BYTES;
  uint32_t ftp2_data_bytes = DEFAULT_DATA_BYTES;
  uint32_t ftp3_data_bytes = DEFAULT_DATA_BYTES;
  uint32_t ftp4_data_bytes = DEFAULT_DATA_BYTES;
  uint32_t ftp5_data_bytes = DEFAULT_DATA_BYTES;  

  double DEFAULT_START_TIME = 0.0;
  double DEFAULT_END_TIME = 20;

  double simEndTime = DEFAULT_END_TIME;

  double ftpApp1StartTime = DEFAULT_START_TIME;
  double ftpApp1EndTime = DEFAULT_END_TIME;
  double ftpApp2StartTime = DEFAULT_START_TIME;
  double ftpApp2EndTime = DEFAULT_END_TIME;
  double ftpApp3StartTime = DEFAULT_START_TIME;
  double ftpApp3EndTime = DEFAULT_END_TIME;
  double ftpApp4StartTime = DEFAULT_START_TIME;
  double ftpApp4EndTime = DEFAULT_END_TIME;
  double ftpApp5StartTime = DEFAULT_START_TIME;
  double ftpApp5EndTime = DEFAULT_END_TIME;


  double sinkApps1StartTime = DEFAULT_START_TIME;
  double sinkApps1EndTime = DEFAULT_END_TIME;
  double sinkApps2StartTime = DEFAULT_START_TIME;
  double sinkApps2EndTime = DEFAULT_END_TIME;
  double sinkApps3StartTime = DEFAULT_START_TIME;
  double sinkApps3EndTime = DEFAULT_END_TIME;
  double sinkApps4StartTime = DEFAULT_START_TIME;
  double sinkApps4EndTime = DEFAULT_END_TIME;
  double sinkApps5StartTime = DEFAULT_START_TIME;
  double sinkApps5EndTime = DEFAULT_END_TIME;

  std::string DEFAULT_DELAY = "5ms";
  std::string S1N1Delay = DEFAULT_DELAY;
  std::string S2N1Delay = DEFAULT_DELAY;
  std::string S3N1Delay = DEFAULT_DELAY;
  std::string S4N1Delay = DEFAULT_DELAY;
  std::string S5N1Delay = DEFAULT_DELAY;  
  std::string N1N2Delay = DEFAULT_DELAY;
  std::string N2R1Delay = DEFAULT_DELAY;
  std::string N2R2Delay = DEFAULT_DELAY;
  std::string N2R3Delay = DEFAULT_DELAY;
  std::string N2R4Delay = DEFAULT_DELAY;  
  std::string N2R5Delay = DEFAULT_DELAY;

  std::string DEFAULT_DATARATE = "10Mbps";
  std::string S1N1DataRate = DEFAULT_DATARATE;
  std::string S2N1DataRate = DEFAULT_DATARATE;
  std::string S3N1DataRate = DEFAULT_DATARATE;
  std::string S4N1DataRate = DEFAULT_DATARATE;
  std::string S5N1DataRate = DEFAULT_DATARATE;    
  std::string N1N2DataRate = "5Mbps";
  std::string N2R1DataRate = DEFAULT_DATARATE;
  std::string N2R2DataRate = DEFAULT_DATARATE;
  std::string N2R3DataRate = DEFAULT_DATARATE;
  std::string N2R4DataRate = DEFAULT_DATARATE;
  std::string N2R5DataRate = DEFAULT_DATARATE;  

  uint16_t DEFAULT_MTU = 2064;
  uint16_t S1N1Mtu = DEFAULT_MTU;
  uint16_t S2N1Mtu = DEFAULT_MTU;
  uint16_t S3N1Mtu = DEFAULT_MTU;
  uint16_t S4N1Mtu = DEFAULT_MTU;
  uint16_t S5N1Mtu = DEFAULT_MTU;
  uint16_t N1N2Mtu = DEFAULT_MTU;
  uint16_t N2R1Mtu = DEFAULT_MTU;
  uint16_t N2R2Mtu = DEFAULT_MTU;
  uint16_t N2R3Mtu = DEFAULT_MTU;
  uint16_t N2R4Mtu = DEFAULT_MTU;
  uint16_t N2R5Mtu = DEFAULT_MTU;  

  uint32_t DEFAULT_QUEUE_SIZE = 655350000;
  uint32_t TXQueueSizeS1N1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeS2N1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeS3N1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeS4N1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeS5N1 = DEFAULT_QUEUE_SIZE;  
  uint32_t TXQueueSizeN2N1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeN2R1 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeN2R2 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeN2R3 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeN2R4 = DEFAULT_QUEUE_SIZE;
  uint32_t TXQueueSizeN2R5 = DEFAULT_QUEUE_SIZE;  

  uint32_t firstQueueSize = DEFAULT_QUEUE_SIZE;
  uint32_t secondQueueSize = DEFAULT_QUEUE_SIZE;
  uint32_t thirdQueueSize = DEFAULT_QUEUE_SIZE;
  uint32_t fourthQueueSize = DEFAULT_QUEUE_SIZE;
  uint32_t fifthQueueSize = DEFAULT_QUEUE_SIZE;
  // double_t firstWeight = 0.09091;
  // double_t secondWeight = 0.90909;

  // uint32_t DEFAULT_MEAN_PACKET_SIZE = tcp_adu_size;
  // uint32_t firstMeanPacketSize = DEFAULT_MEAN_PACKET_SIZE;
  // uint32_t secondMeanPacketSize = DEFAULT_MEAN_PACKET_SIZE;

  uint64_t totalExpectedBytes1 = ftp1_data_bytes;
  uint64_t totalExpectedBytes2 = ftp2_data_bytes;
  uint64_t totalExpectedBytes3 = ftp3_data_bytes;
  uint64_t totalExpectedBytes4 = ftp4_data_bytes;
  uint64_t totalExpectedBytes5 = ftp5_data_bytes;

  double bandwidthCalculationInterval = 0.010;
  uint32_t quantumSize1 = 100;
  uint32_t quantumSize2 = 500;
  uint32_t quantumSize3 = 1500;
  uint32_t quantumSize4 = 250;
  uint32_t quantumSize5 = 2500;

  //std::string w = static_cast<std::ostringstream*>( &(std::ostringstream() << int(round(secondWeight/firstWeight))) )->str();


// Allow the user to override any of the defaults at
// run-time, via command-line arguments
  CommandLine cmd;
  cmd.AddValue ("tracing", "Flag to enable/disable tracing", tracing);

  cmd.AddValue ("secondQueuePort", "Destination port for second queue traffic", secondQueuePort);
  cmd.AddValue ("R1port", "R1 port", R1port);
  cmd.AddValue ("R2port", "R2 port", R2port);
  cmd.AddValue ("R3port", "R3 port", R3port);
  cmd.AddValue ("R4port", "R4 port", R4port);
  cmd.AddValue ("R5port", "R5 port", R5port);   
  cmd.AddValue ("tcp_adu_size", "Packet payload size", tcp_adu_size); // Actually, this is the maximum payload size, but the BulkSender application sends packets only at maximum size. So this technically sets the size of our packet's payload.

  cmd.AddValue ("ftp1_dataBytes", "Total number of bytes for first FTP application to send", ftp1_data_bytes);
  cmd.AddValue ("ftp2_dataBytes", "Total number of bytes for second FTP application to send", ftp2_data_bytes);
  cmd.AddValue ("ftp3_dataBytes", "Total number of bytes for first FTP application to send", ftp3_data_bytes);
  cmd.AddValue ("ftp4_dataBytes", "Total number of bytes for second FTP application to send", ftp4_data_bytes);  
  cmd.AddValue ("ftp5_dataBytes", "Total number of bytes for second FTP application to send", ftp5_data_bytes);

  cmd.AddValue ("simEndTime", "Simulation end time", simEndTime);

  cmd.AddValue ("ftpApp1StartTime", "First FTP application start time", ftpApp1StartTime);
  cmd.AddValue ("ftpApp1EndTime", "First FTP application end time", ftpApp1EndTime);
  cmd.AddValue ("ftpApp2StartTime", "Second FTP application start time", ftpApp2StartTime);
  cmd.AddValue ("ftpApp2EndTime", "Second FTP application end time", ftpApp2EndTime);
  cmd.AddValue ("ftpApp3StartTime", "First FTP application start time", ftpApp3StartTime);
  cmd.AddValue ("ftpApp3EndTime", "First FTP application end time", ftpApp3EndTime);
  cmd.AddValue ("ftpApp4StartTime", "Second FTP application start time", ftpApp4StartTime);
  cmd.AddValue ("ftpApp4EndTime", "Second FTP application end time", ftpApp4EndTime);
  cmd.AddValue ("ftpApp5StartTime", "Second FTP application start time", ftpApp5StartTime);
  cmd.AddValue ("ftpApp5EndTime", "Second FTP application end time", ftpApp5EndTime);


  cmd.AddValue ("sinkApps1StartTime", "First receiver application start time", sinkApps1StartTime);
  cmd.AddValue ("sinkApps1EndTime", "First receiver application end time", sinkApps1EndTime);
  cmd.AddValue ("sinkApps2StartTime", "Second receiver application start time", sinkApps2StartTime);
  cmd.AddValue ("sinkApps2EndTime", "Second receiver application end time", sinkApps2EndTime);
  cmd.AddValue ("sinkApps3StartTime", "First receiver application start time", sinkApps3StartTime);
  cmd.AddValue ("sinkApps3EndTime", "First receiver application end time", sinkApps3EndTime);
  cmd.AddValue ("sinkApps4StartTime", "Second receiver application start time", sinkApps4StartTime);
  cmd.AddValue ("sinkApps4EndTime", "Second receiver application end time", sinkApps4EndTime);
  cmd.AddValue ("sinkApps5StartTime", "Second receiver application start time", sinkApps5StartTime);
  cmd.AddValue ("sinkApps5EndTime", "Second receiver application end time", sinkApps5EndTime);

  cmd.AddValue ("S1N1Delay", "The transmission delay of the p2pchannel S1N1", S1N1Delay);
  cmd.AddValue ("S2N1Delay", "The transmission delay of the p2pchannel S2N1", S2N1Delay);
  cmd.AddValue ("S3N1Delay", "The transmission delay of the p2pchannel S1N1", S3N1Delay);
  cmd.AddValue ("S4N1Delay", "The transmission delay of the p2pchannel S2N1", S4N1Delay);
  cmd.AddValue ("S5N1Delay", "The transmission delay of the p2pchannel S2N1", S5N1Delay);  
  cmd.AddValue ("N1N2Delay", "The transmission delay of the p2pchannel N1N2", N1N2Delay);
  cmd.AddValue ("N2R1Delay", "The transmission delay of the p2pchannel N2R1", N2R1Delay);
  cmd.AddValue ("N2R2Delay", "The transmission delay of the p2pchannel N2R2", N2R2Delay);
  cmd.AddValue ("N2R3Delay", "The transmission delay of the p2pchannel N2R1", N2R3Delay);
  cmd.AddValue ("N2R4Delay", "The transmission delay of the p2pchannel N2R2", N2R4Delay);
  cmd.AddValue ("N2R5Delay", "The transmission delay of the p2pchannel N2R2", N2R5Delay);    


  cmd.AddValue ("S1N1DataRate", "The transmission rate on p2pchannel S1N1", S1N1DataRate);
  cmd.AddValue ("S2N1DataRate", "The transmission rate on p2pchannel S2N1", S2N1DataRate);
  cmd.AddValue ("S3N1DataRate", "The transmission rate on p2pchannel S1N1", S3N1DataRate);
  cmd.AddValue ("S4N1DataRate", "The transmission rate on p2pchannel S2N1", S4N1DataRate);
  cmd.AddValue ("S5N1DataRate", "The transmission rate on p2pchannel S2N1", S5N1DataRate);  
  cmd.AddValue ("N1N2DataRate", "The transmission rate on p2pchannel N1N2", N1N2DataRate);
  cmd.AddValue ("N2R1DataRate", "The transmission rate on p2pchannel N2R1", N2R1DataRate);
  cmd.AddValue ("N2R2DataRate", "The transmission rate on p2pchannel N2R2", N2R2DataRate);
  cmd.AddValue ("N2R3DataRate", "The transmission rate on p2pchannel N2R1", N2R3DataRate);
  cmd.AddValue ("N2R4DataRate", "The transmission rate on p2pchannel N2R2", N2R4DataRate);
  cmd.AddValue ("N2R5DataRate", "The transmission rate on p2pchannel N2R2", N2R5DataRate);    

  cmd.AddValue ("S1N1Mtu", "S1N1 MTU", S1N1Mtu);
  cmd.AddValue ("S2N1Mtu", "S2N1 MTU", S2N1Mtu);
  cmd.AddValue ("S3N1Mtu", "S3N1 MTU", S3N1Mtu);
  cmd.AddValue ("S4N1Mtu", "S4N1 MTU", S4N1Mtu);
  cmd.AddValue ("S5N1Mtu", "S5N1 MTU", S5N1Mtu);  
  cmd.AddValue ("N1N2Mtu", "N1N2 MTU", N1N2Mtu);
  cmd.AddValue ("N2R1Mtu", "N2R1 MTU", N2R1Mtu);
  cmd.AddValue ("N2R2Mtu", "N2R2 MTU", N2R2Mtu);
  cmd.AddValue ("N2R3Mtu", "N2R3 MTU", N2R3Mtu);
  cmd.AddValue ("N2R4Mtu", "N2R4 MTU", N2R4Mtu);
  cmd.AddValue ("N2R5Mtu", "N2R5 MTU", N2R5Mtu);    

  cmd.AddValue ("TXQueueSizeS1N1", "The size of the outgoing queue on S1N1", TXQueueSizeS1N1);
  cmd.AddValue ("TXQueueSizeS2N1", "The size of the outgoing queue on S2N1", TXQueueSizeS2N1);
  cmd.AddValue ("TXQueueSizeS3N1", "The size of the outgoing queue on S3N1", TXQueueSizeS3N1);
  cmd.AddValue ("TXQueueSizeS4N1", "The size of the outgoing queue on S4N1", TXQueueSizeS4N1);
  cmd.AddValue ("TXQueueSizeS5N1", "The size of the outgoing queue on S5N1", TXQueueSizeS5N1);  
  cmd.AddValue ("TXQueueSizeN2N1", "The size of the outgoing queue on N2N1", TXQueueSizeN2N1);
  cmd.AddValue ("TXQueueSizeN2R1", "The size of the outgoing queue on N2R1", TXQueueSizeN2R1);
  cmd.AddValue ("TXQueueSizeN2R2", "The size of the outgoing queue on N2R2", TXQueueSizeN2R2);
  cmd.AddValue ("TXQueueSizeN2R3", "The size of the outgoing queue on N2R3", TXQueueSizeN2R3);
  cmd.AddValue ("TXQueueSizeN2R4", "The size of the outgoing queue on N2R4", TXQueueSizeN2R4);  
  cmd.AddValue ("TXQueueSizeN2R5", "The size of the outgoing queue on N2R5", TXQueueSizeN2R5);
  cmd.AddValue ("firstQueueSize", "The size of the first weighted queue", firstQueueSize);
  cmd.AddValue ("secondQueueSize", "The size of the second weighted queue", secondQueueSize);
  cmd.AddValue ("thirdQueueSize", "The size of the third weighted queue", thirdQueueSize);
  cmd.AddValue ("fourthQueueSize", "The size of the fourt weighted queue", fourthQueueSize);
  cmd.AddValue ("fifthQueueSize", "The size of the fifth weighted queue", fifthQueueSize);
  // cmd.AddValue ("firstWeight", "The weight of the first queue, 0<w<1", firstWeight);
  // cmd.AddValue ("secondWeight", "The weight of the second queue, 0<w<1", secondWeight);

  // cmd.AddValue ("firstMeanPacketSize", "The mean packet size of first queue", firstMeanPacketSize);
  // cmd.AddValue ("secondMeanPacketSize", "The mean packet size of second queue", secondMeanPacketSize);

  cmd.AddValue ("totalExpectedBytes1", "Total expected bytes passed before end of simulation - used for showing progression percentage", totalExpectedBytes1);
  cmd.AddValue ("totalExpectedBytes2", "Total expected bytes passed before end of simulation - used for showing progression percentage", totalExpectedBytes2);
  cmd.AddValue ("totalExpectedBytes3", "Total expected bytes passed before end of simulation - used for showing progression percentage", totalExpectedBytes3);
  cmd.AddValue ("totalExpectedBytes4", "Total expected bytes passed before end of simulation - used for showing progression percentage", totalExpectedBytes4);
  cmd.AddValue ("totalExpectedBytes5", "Total expected bytes passed before end of simulation - used for showing progression percentage", totalExpectedBytes5);
       
  cmd.AddValue ("quantumSize1", "Simulation end time", quantumSize1);
  cmd.AddValue ("quantumSize2", "Simulation end time", quantumSize2);
  cmd.AddValue ("quantumSize3", "Simulation end time", quantumSize3);
  cmd.AddValue ("quantumSize4", "Simulation end time", quantumSize4);
  cmd.AddValue ("quantumSize5", "Simulation end time", quantumSize5);

  cmd.Parse (argc, argv);


// creating the topology
  NodeContainer allNodes;
  allNodes.Create(12);

  NodeContainer S1 = NodeContainer (allNodes.Get(0));
  NodeContainer S2 = NodeContainer (allNodes.Get(1));
  NodeContainer S3 = NodeContainer (allNodes.Get(2));
  NodeContainer S4 = NodeContainer (allNodes.Get(3));
  NodeContainer S5 = NodeContainer (allNodes.Get(4));  
  NodeContainer N1 = NodeContainer (allNodes.Get(5));
  NodeContainer N2 = NodeContainer (allNodes.Get(6));
  NodeContainer R1 = NodeContainer (allNodes.Get(7));
  NodeContainer R2 = NodeContainer (allNodes.Get(8));
  NodeContainer R3 = NodeContainer (allNodes.Get(9));
  NodeContainer R4 = NodeContainer (allNodes.Get(10));
  NodeContainer R5 = NodeContainer (allNodes.Get(11));    

  NodeContainer S1N1 = NodeContainer (S1, N1);
  NodeContainer S2N1 = NodeContainer (S2, N1);
  NodeContainer S3N1 = NodeContainer (S3, N1);
  NodeContainer S4N1 = NodeContainer (S4, N1);
  NodeContainer S5N1 = NodeContainer (S5, N1);  
  NodeContainer N1N2 = NodeContainer (N1, N2);
  NodeContainer N2R1 = NodeContainer (N2, R1);
  NodeContainer N2R2 = NodeContainer (N2, R2);
  NodeContainer N2R3 = NodeContainer (N2, R3);
  NodeContainer N2R4 = NodeContainer (N2, R4);  
  NodeContainer N2R5 = NodeContainer (N2, R5);

  // Create the point to point net devices and channels
  PointToPointHelper p2p;
  p2p.SetQueue("ns3::DropTailQueue", "Mode", (StringValue) "QUEUE_MODE_PACKETS");

  //S1N1
  p2p.SetChannelAttribute ("Delay", (StringValue) S1N1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) S1N1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (S1N1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeS1N1));
  NetDeviceContainer S1N1_d = p2p.Install (S1N1);

  //S2N1
  p2p.SetChannelAttribute ("Delay", (StringValue) S2N1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) S2N1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (S2N1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeS2N1));
  NetDeviceContainer S2N1_d = p2p.Install (S2N1);

  //S3N1
  p2p.SetChannelAttribute ("Delay", (StringValue) S3N1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) S3N1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (S3N1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeS3N1));
  NetDeviceContainer S3N1_d = p2p.Install (S3N1);

  //S4N1
  p2p.SetChannelAttribute ("Delay", (StringValue) S4N1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) S4N1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (S4N1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeS4N1));
  NetDeviceContainer S4N1_d = p2p.Install (S4N1);  

  //S5N1
  p2p.SetChannelAttribute ("Delay", (StringValue) S5N1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) S5N1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (S5N1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeS5N1));
  NetDeviceContainer S5N1_d = p2p.Install (S5N1);   

  //N1N2: WRR on N1, regular tail drop queue on N2
  p2p.SetChannelAttribute ("Delay", (StringValue) N1N2Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N1N2DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N1N2Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2N1));
  NetDeviceContainer N1N2_d = p2p.Install (N1N2);

  ObjectFactory m_queueFactory;
  m_queueFactory.SetTypeId ("ns3::DeficitRoundRobin");
  m_queueFactory.Set ("firstMaxPackets", UintegerValue (firstQueueSize));
  m_queueFactory.Set ("secondMaxPackets", UintegerValue (secondQueueSize));
  m_queueFactory.Set ("thirdMaxPackets", UintegerValue (thirdQueueSize));
  m_queueFactory.Set ("fourthMaxPackets", UintegerValue (fourthQueueSize));
  m_queueFactory.Set ("fifthMaxPackets", UintegerValue (fifthQueueSize));  
  m_queueFactory.Set ("QuantumSize1", UintegerValue (quantumSize1));
  m_queueFactory.Set ("QuantumSize2", UintegerValue (quantumSize2));
  m_queueFactory.Set ("QuantumSize3", UintegerValue (quantumSize3));
  m_queueFactory.Set ("QuantumSize4", UintegerValue (quantumSize4));
  m_queueFactory.Set ("QuantumSize5", UintegerValue (quantumSize5));


  //Ptr<BaseClass> queueN1 = m_queueFactory.Create<BaseClass> ();
  //Ptr<PointToPointNetDevice>  devN1 = NetDeviceDynamicCast(N1N2_d.Get(0));
  //devN1->SetQueue (queueN1);


  //N2R1
  p2p.SetChannelAttribute ("Delay", (StringValue) N2R1Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N2R1DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N2R1Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2R1));
  NetDeviceContainer N2R1_d = p2p.Install (N2R1);

  //N2R2
  p2p.SetChannelAttribute ("Delay", (StringValue) N2R2Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N2R2DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N2R2Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2R2));
  NetDeviceContainer N2R2_d = p2p.Install (N2R2);

  //N2R3
  p2p.SetChannelAttribute ("Delay", (StringValue) N2R3Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N2R3DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N2R3Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2R3));
  NetDeviceContainer N2R3_d = p2p.Install (N2R3);

  //N2R4
  p2p.SetChannelAttribute ("Delay", (StringValue) N2R4Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N2R4DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N2R4Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2R4));
  NetDeviceContainer N2R4_d = p2p.Install (N2R4);  

  //N2R5
  p2p.SetChannelAttribute ("Delay", (StringValue) N2R5Delay);
  p2p.SetDeviceAttribute ("DataRate", (StringValue) N2R5DataRate);
  p2p.SetDeviceAttribute ("Mtu", UintegerValue (N2R5Mtu));
  p2p.SetQueue("ns3::DropTailQueue", "MaxPackets", UintegerValue (TXQueueSizeN2R5));
  NetDeviceContainer N2R5_d = p2p.Install (N2R5); 

  // Create the Internet stacks
  InternetStackHelper stack;
  stack.Install (allNodes);


  // Create addresses
  Ipv4AddressHelper ipv4;

  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer S1N1_i = ipv4.Assign (S1N1_d);

  ipv4.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer S2N1_i = ipv4.Assign (S2N1_d);

  ipv4.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer S3N1_i = ipv4.Assign (S3N1_d);

  ipv4.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer S4N1_i = ipv4.Assign (S4N1_d);  

  ipv4.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer S5N1_i = ipv4.Assign (S5N1_d);  

  ipv4.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer N1_i = ipv4.Assign (N1N2_d);

  ipv4.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer N2R1_i = ipv4.Assign (N2R1_d);

  ipv4.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer N2R2_i = ipv4.Assign (N2R2_d);

  ipv4.SetBase ("10.1.9.0", "255.255.255.0");
  Ipv4InterfaceContainer N2R3_i = ipv4.Assign (N2R3_d);

  ipv4.SetBase ("10.1.10.0", "255.255.255.0");
  Ipv4InterfaceContainer N2R4_i = ipv4.Assign (N2R4_d);  

  ipv4.SetBase ("10.1.11.0", "255.255.255.0");
  Ipv4InterfaceContainer N2R5_i = ipv4.Assign (N2R5_d);   


  // Enable routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();


  // Create applications

  // create a FTP application and install it on S1
  //Config::SetDefault ("ns3::TcpSocket::SegmentSize", UintegerValue (tcp_adu_size));
  AddressValue R1Address (InetSocketAddress (N2R1_i.GetAddress (1), R1port));
  BulkSendHelper ftp ("ns3::TcpSocketFactory", Address ());
  ftp.SetAttribute ("Remote", R1Address);
  ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size));
  ftp.SetAttribute ("MaxBytes", UintegerValue (ftp1_data_bytes));
  //ftp.SetAttribute ("SegmentSize",UintegerValue(tcp_adu_size));
  ApplicationContainer ftpApp1 = ftp.Install (S1.Get (0));

  ftpApp1.Start (Seconds (ftpApp1StartTime));
  ftpApp1.Stop (Seconds (ftpApp1EndTime));


  // create a FTP application and install it on S2
  AddressValue R2Address (InetSocketAddress (N2R2_i.GetAddress (1), R2port));
  ftp.SetAttribute ("Remote", R2Address);
  ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size ));
  ftp.SetAttribute ("MaxBytes", UintegerValue (ftp2_data_bytes));
  //ftp.SetAttribute ("SegmentSize",UintegerValue(tcp_adu_size));
  ApplicationContainer ftpApp2 = ftp.Install (S2.Get (0));

  ftpApp2.Start (Seconds (ftpApp2StartTime));
  ftpApp2.Stop (Seconds (ftpApp2EndTime));


  // create a FTP application and install it on S3
  AddressValue R3Address (InetSocketAddress (N2R3_i.GetAddress (1), R3port));
  ftp.SetAttribute ("Remote", R3Address);
  ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size));
  ftp.SetAttribute ("MaxBytes", UintegerValue (ftp3_data_bytes));
  //ftp.SetAttribute ("SegmentSize",UintegerValue(tcp_adu_size));
  ApplicationContainer ftpApp3 = ftp.Install (S3.Get (0));

  ftpApp3.Start (Seconds (ftpApp3StartTime));
  ftpApp3.Stop (Seconds (ftpApp3EndTime));


  // create a FTP application and install it on 4
  AddressValue R4Address (InetSocketAddress (N2R4_i.GetAddress (1), R4port));
  ftp.SetAttribute ("Remote", R4Address);
  ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size));
  ftp.SetAttribute ("MaxBytes", UintegerValue (ftp4_data_bytes));
  //ftp.SetAttribute ("SegmentSize",UintegerValue(tcp_adu_size));
  ApplicationContainer ftpApp4 = ftp.Install (S4.Get (0));

  ftpApp4.Start (Seconds (ftpApp4StartTime));
  ftpApp4.Stop (Seconds (ftpApp4EndTime));  

  // create a FTP application and install it on 5
  AddressValue R5Address (InetSocketAddress (N2R5_i.GetAddress (1), R5port));
  ftp.SetAttribute ("Remote", R5Address);
  ftp.SetAttribute ("SendSize", UintegerValue (tcp_adu_size ));
  ftp.SetAttribute ("MaxBytes", UintegerValue (ftp5_data_bytes));
  //ftp.SetAttribute ("SegmentSize",UintegerValue(tcp_adu_size));
  ApplicationContainer ftpApp5 = ftp.Install (S5.Get (0));

  ftpApp5.Start (Seconds (ftpApp5StartTime));
  ftpApp5.Stop (Seconds (ftpApp5EndTime));   


  ObjectFactory m_factory1;
  m_factory1.SetTypeId ("ns3::CustomPacketSink");
  m_factory1.Set ("Protocol", StringValue ("ns3::TcpSocketFactory"));
  m_factory1.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), R1port)));
  m_factory1.Set ("TotalExpectedRx", UintegerValue(totalExpectedBytes1));
  m_factory1.Set ("BandwidthInterval", TimeValue( Seconds (bandwidthCalculationInterval)));
  m_factory1.Set ("ReceiverName", StringValue(DEFAULT_DATARATE+"-w"+"-R1.txt"));
  Ptr<Application> app1 = m_factory1.Create<Application> ();
  R1.Get (0)->AddApplication (app1);
  ApplicationContainer sinkApps1;
  sinkApps1.Add(app1);

  sinkApps1.Start (Seconds (sinkApps1StartTime));
  sinkApps1.Stop (Seconds (sinkApps1EndTime));

  ObjectFactory m_factory2;
  m_factory2.SetTypeId ("ns3::CustomPacketSink");
  m_factory2.Set ("Protocol", StringValue ("ns3::TcpSocketFactory"));
  m_factory2.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), R2port)));
  m_factory2.Set ("TotalExpectedRx", UintegerValue(totalExpectedBytes2));
  m_factory2.Set ("BandwidthInterval", TimeValue( Seconds (bandwidthCalculationInterval)));
  m_factory2.Set ("ReceiverName", StringValue(DEFAULT_DATARATE+"-w"+"-R2.txt"));
  Ptr<Application> app2 = m_factory2.Create<Application> ();
  R2.Get (0)->AddApplication (app2);
  ApplicationContainer sinkApps2;
  sinkApps2.Add(app2);

  sinkApps2.Start (Seconds (sinkApps2StartTime));
  sinkApps2.Stop (Seconds (sinkApps2EndTime));


  ObjectFactory m_factory3;
  m_factory3.SetTypeId ("ns3::CustomPacketSink");
  m_factory3.Set ("Protocol", StringValue ("ns3::TcpSocketFactory"));
  m_factory3.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), R3port)));
  m_factory3.Set ("TotalExpectedRx", UintegerValue(totalExpectedBytes3));
  m_factory3.Set ("BandwidthInterval", TimeValue( Seconds (bandwidthCalculationInterval)));
  m_factory3.Set ("ReceiverName", StringValue(DEFAULT_DATARATE+"-w"+"-R3.txt"));
  Ptr<Application> app3 = m_factory3.Create<Application> ();
  R3.Get (0)->AddApplication (app3);
  ApplicationContainer sinkApps3;
  sinkApps3.Add(app3);

  sinkApps3.Start (Seconds (sinkApps3StartTime));
  sinkApps3.Stop (Seconds (sinkApps3EndTime));

  ObjectFactory m_factory4;
  m_factory4.SetTypeId ("ns3::CustomPacketSink");
  m_factory4.Set ("Protocol", StringValue ("ns3::TcpSocketFactory"));
  m_factory4.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), R4port)));
  m_factory4.Set ("TotalExpectedRx", UintegerValue(totalExpectedBytes4));
  m_factory4.Set ("BandwidthInterval", TimeValue( Seconds (bandwidthCalculationInterval)));
  m_factory4.Set ("ReceiverName", StringValue(DEFAULT_DATARATE+"-w"+"-R4.txt"));
  Ptr<Application> app4 = m_factory4.Create<Application> ();
  R4.Get (0)->AddApplication (app4);
  ApplicationContainer sinkApps4;
  sinkApps4.Add(app4);

  sinkApps4.Start (Seconds (sinkApps4StartTime));
  sinkApps4.Stop (Seconds (sinkApps4EndTime));  

  ObjectFactory m_factory5;
  m_factory5.SetTypeId ("ns3::CustomPacketSink");
  m_factory5.Set ("Protocol", StringValue ("ns3::TcpSocketFactory"));
  m_factory5.Set ("Local", AddressValue (InetSocketAddress (Ipv4Address::GetAny (), R5port)));
  m_factory5.Set ("TotalExpectedRx", UintegerValue(totalExpectedBytes5));
  m_factory5.Set ("BandwidthInterval", TimeValue( Seconds (bandwidthCalculationInterval)));
  m_factory5.Set ("ReceiverName", StringValue(DEFAULT_DATARATE+"-w"+"-R5.txt"));
  Ptr<Application> app5 = m_factory5.Create<Application> ();
  R5.Get (0)->AddApplication (app5);
  ApplicationContainer sinkApps5;
  sinkApps5.Add(app5);

  sinkApps5.Start (Seconds (sinkApps5StartTime));
  sinkApps5.Stop (Seconds (sinkApps5EndTime));   


// Set up tracing if enabled
  if (tracing)
    {
      AsciiTraceHelper ascii;
      p2p.EnableAscii (ascii.CreateFileStream ("drr-r1.tr"), R1);
      p2p.EnableAscii (ascii.CreateFileStream ("drr-r2.tr"), R2);
      p2p.EnableAscii (ascii.CreateFileStream ("drr-r3.tr"), R3);
      p2p.EnableAscii (ascii.CreateFileStream ("drr-r4.tr"), R4);
      p2p.EnableAscii (ascii.CreateFileStream ("drr-r5.tr"), R5);      

	  p2p.EnablePcap (DEFAULT_DATARATE+"-w"+"-DRR1", R1, false);
      p2p.EnablePcap (DEFAULT_DATARATE+"-w"+"-DRR2", R2, false);
	  p2p.EnablePcap (DEFAULT_DATARATE+"-w"+"-DRR3", R3, false);
      p2p.EnablePcap (DEFAULT_DATARATE+"-w"+"-DRR4", R4, false);
      p2p.EnablePcap (DEFAULT_DATARATE+"-w"+"-DRR5", R5, false);             

      //p2p.EnablePcap (DEFAULT_DATARATE+"-w"+w+"-N2", N2, false);
    }


// Now, do the actual simulation.
  Simulator::Stop (Seconds (simEndTime));

  //FlowMonitorHelper flowmon;
  //Ptr<FlowMonitor> monitor = flowmon.Install( allNodes );

  Simulator::Run ();

  //monitor->SerializeToXmlFile ("results-"+DEFAULT_DATARATE+"-w"+w+".xml", true, true);

  Simulator::Destroy ();


  Ptr<CustomPacketSink> packetSink1 = DynamicCast<CustomPacketSink> (sinkApps1.Get (0));
  std::cout << "Total Bytes Received at R1: " << packetSink1->GetTotalRx () << std::endl;

  Ptr<CustomPacketSink> packetSink2 = DynamicCast<CustomPacketSink> (sinkApps2.Get (0));
  std::cout << "Total Bytes Received at R2: " << packetSink2->GetTotalRx () << std::endl;

  Ptr<CustomPacketSink> packetSink3 = DynamicCast<CustomPacketSink> (sinkApps3.Get (0));
  std::cout << "Total Bytes Received at R3: " << packetSink3->GetTotalRx () << std::endl;

  Ptr<CustomPacketSink> packetSink4 = DynamicCast<CustomPacketSink> (sinkApps4.Get (0));
  std::cout << "Total Bytes Received at R4: " << packetSink4->GetTotalRx () << std::endl;

    Ptr<CustomPacketSink> packetSink5 = DynamicCast<CustomPacketSink> (sinkApps5.Get (0));
  std::cout << "Total Bytes Received at R5: " << packetSink5->GetTotalRx () << std::endl;   

  //std::cout << "Attained Weight is: " << double(packetSink2->GetTotalRx ()) / double(packetSink1->GetTotalRx ()) << std::endl;

  return 0;
}
