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
 
int main(){

  srand (time(NULL));
  rand();

  std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

  uint32_t header_words[3];
  RAWDAQHeader header(&header_words[0]);

  //  std::chrono::nanoseconds ns(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()))

  std::vector<uint32_t> data;
  data.reserve(50000);
  //std::chrono::high_resolution_clock::time_point t1;
  uint64_t ns;  
  for(size_t i=0; i < 100; i++){

    //t1 = std::chrono::high_resolution_clock::now();
    ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() * 4;

    IDODSync tmp((uint32_t)(ns >>16));
    //    std::cout<<std::bitset<9>(tmp.GetChannel())<<std::endl;
    data.push_back(tmp.GetData()[0]);
    data.push_back(tmp.GetData()[1]);

    for( size_t j=0; j< 240; j++){
      ns=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-start).count() * 4;

      IDODHit tmp2((uint8_t)ns, (uint16_t)(ns>>16));
      data.push_back(tmp2.GetData()->at(0));
      data.push_back(tmp2.GetData()->at(1));
    }


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
