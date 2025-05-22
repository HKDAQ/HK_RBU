#include <iostream>
#include <RAWDAQHeader.h>
#include <RAWIDODHit.h>
#include <RAWIDODPed.h>
#include <RAWIDODSync.h>
#include <RAWIDODError.h>
#include <RAWIDODSpecial.h>

#include <stddef.h>
#include <cstdint>
#include <vector>
#include <chrono>
#include <bitset>
#include <stdlib.h>     /* srand, rand */
#include <zmq.hpp>
 
int main(int argc, char *argv[]){

  srand (time(NULL));
  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
  std::chrono::high_resolution_clock::time_point last = start;
  uint32_t message_num=0;
  
  DAQHeader header;
  uint64_t ps;
  uint64_t ms;
  std::vector<uint32_t> data;
  data.reserve(50000);
  
  zmq::context_t context(20);
  zmq::socket_t sock(context, ZMQ_DEALER);
  sock.setsockopt(ZMQ_SNDHWM, 3);
  std::string port = argv[1];
  sock.bind("tcp://*:" + port);

    ms=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-last).count();
    
       last = std::chrono::high_resolution_clock::now();
      ps=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() * 4;
      
      data.clear();
      
      header.SetMessageNum(message_num);
      message_num++;
      header.SetCoarseCounter((uint32_t)ps);
      header.SetCardID(rand() % 2000);
      
      for(size_t i=0; i < 100; i++){
	
	//t1 = std::chrono::high_resolution_clock::now();
	ps=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() * 4;
	
	IDODSync tmp((uint32_t)(ps >>16));
	//    std::cout<<std::bitset<9>(tmp.GetChannel())<<std::endl;
	data.push_back(tmp.GetData()[0]);
	data.push_back(tmp.GetData()[1]);
	
	for( size_t j=0; j< 24000; j++){
	  ps=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() * 4;
	  
	  IDODHit tmp2((uint8_t)ps, (uint16_t)(ps>>16));
	  tmp2.SetPed(false);
	  data.push_back(tmp2.GetData()->at(0));
	  data.push_back(tmp2.GetData()->at(1));
	}	
      }


  
  while(true){
    ms=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-last).count();
  last = std::chrono::high_resolution_clock::now();

  zmq::message_t msg1(sizeof(header));
  std::memcpy(msg1.data(), &header.GetData()[0], sizeof(header));

      zmq::message_t msg2(data.size()*sizeof(uint32_t));
      std::memcpy(msg2.data(), data.data(), data.size()*sizeof(uint32_t));

      // sock.send(msg1, ZMQ_SNDMORE | ZMQ_NOBLOCK);
      // sock.send(msg2, ZMQ_NOBLOCK);

      sock.send(msg1, ZMQ_SNDMORE);
      sock.send(msg2);

      std::cout<<"sent data: "<<ms<<std::endl;


    }



    
  for(size_t i=0; i<10; i=i+2){ //data.size(); i++){
    
    RAWIDODHit tmp(&data.at(i));
    tmp.Print();
//    std::bitset<32> tmp(data.at(i));
    //std::bitset<32> tmp2(data.at(i+1));
    //std::cout<<i<<": "<<data.at(i)<<", "<<data.at(i+1)<<": "<<tmp<<" "<<tmp2<<std::endl;

  }





  return 0;

}
