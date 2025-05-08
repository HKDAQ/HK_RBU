#include <RAWDAQHeader.h>

namespace {
  constexpr uint32_t TYPE_MASK      = 0b11000000000000000000000000000000;
  constexpr uint32_t CAR_ID_MASK    = 0b00111111111111000000000000000000;
  constexpr uint32_t WORDS_MASK     = 0b00000000000000111111111111111000;
  constexpr uint32_t FLAGS_MASK     = 0b00000000000000000000000000000111;
}

RAWDAQHeader::RAWDAQHeader(){;}

RAWDAQHeader::RAWDAQHeader(uint32_t* in_data){ data=in_data;}

uint32_t RAWDAQHeader::GetMessageNum() const { return data[0]; }
uint32_t RAWDAQHeader::GetCoarseCounter()const { return data[1]; }
uint8_t RAWDAQHeader::GetType() const { return data[2] >> 30;}
uint16_t RAWDAQHeader::GetCardID() const { return ((data[2] & CAR_ID_MASK) >> 18);}
uint16_t RAWDAQHeader::GetNumWords() const { return ((data[2] & WORDS_MASK) >> 3);}
uint8_t RAWDAQHeader::GetFlags() const{ return data[2] & FLAGS_MASK; }
void RAWDAQHeader::Print() const{

  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<" , "<<std::bitset<32>(data[2])<<std::endl;
  std::cout<<"MessageNum: "<<GetMessageNum()<<" , "<<std::bitset<32>(GetMessageNum())<<std::endl;
  std::cout<<"CoarseCounter: "<<GetCoarseCounter()<<" , "<<std::bitset<32>(GetCoarseCounter())<<std::endl;
  std::cout<<"Type: "<<+GetType()<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"CardID: "<<GetCardID()<<" , "<<std::bitset<12>(GetCardID())<<std::endl;
  std::cout<<"NumWords: "<<GetNumWords()<<" , "<<std::bitset<15>(GetNumWords())<<std::endl;
  std::cout<<"Flags: "<<+GetFlags()<<" , "<<std::bitset<3>(GetFlags())<<std::endl;

}


DAQHeader::DAQHeader(){;}

DAQHeader::DAQHeader(uint32_t coarse_counter, uint32_t msg_num){ 

  SetMessageNum(msg_num);  
  data[1] = rand() % UINT_MAX;
  data[2] = rand() % UINT_MAX;
  SetCoarseCounter(coarse_counter);

}

uint32_t DAQHeader::GetMessageNum() const { return data[0]; }
uint32_t DAQHeader::GetCoarseCounter()const { return data[1]; }
uint8_t DAQHeader::GetType() const { return data[2] >> 30;}
uint16_t DAQHeader::GetCardID() const { return ((data[2] & CAR_ID_MASK) >> 18);}
uint16_t DAQHeader::GetNumWords() const { return ((data[2] & WORDS_MASK) >> 3);}
uint8_t DAQHeader::GetFlags() const { return data[2] & FLAGS_MASK;}
uint32_t* DAQHeader::GetData(){ return &data[0];}

void DAQHeader::SetMessageNum(uint32_t in){ data[0] = in; }
void DAQHeader::SetCoarseCounter(uint32_t in){ data[1] = in; }
void DAQHeader::SetType(uint8_t in){ data[2] = (data[2] & ~TYPE_MASK) | (in & 0b11); } 
void DAQHeader::SetCardID(uint16_t in){ data[2] = (data[2] & ~CAR_ID_MASK) | (in & 0b111111111111); }
void DAQHeader::SetNumWords(uint16_t in){ data[2] = (data[2] & ~WORDS_MASK) | (in & 0b111111111111111); }
void DAQHeader::SetFlags(uint8_t in){ data[2] = (data[2] & ~FLAGS_MASK); }

void DAQHeader::Print() const{

  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<" , "<<std::bitset<32>(data[2])<<std::endl;
  std::cout<<"MessageNum: "<<GetMessageNum()<<" , "<<std::bitset<32>(GetMessageNum())<<std::endl;
  std::cout<<"CoarseCounter: "<<GetCoarseCounter()<<" , "<<std::bitset<32>(GetCoarseCounter())<<std::endl;
  std::cout<<"Type: "<<+GetType()<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"CardID: "<<GetCardID()<<" , "<<std::bitset<12>(GetCardID())<<std::endl;
  std::cout<<"NumWords: "<<GetNumWords()<<" , "<<std::bitset<15>(GetNumWords())<<std::endl;
  std::cout<<"Flags: "<<+GetFlags()<<" , "<<std::bitset<3>(GetFlags())<<std::endl;

}
