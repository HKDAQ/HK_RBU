#include <RAWIDODHit.h>

namespace {
  constexpr uint32_t TYPE_MASK      = 0b11000000000000000000000000000000;
  constexpr uint32_t BOARD_NUM_MASK  = 0b00100000000000000000000000000000;
  constexpr uint32_t IDOD_MASK      = 0b00010000000000000000000000000000;
  constexpr uint32_t CHANNEL_MASK   = 0b00001111000000000000000000000000;
  constexpr uint32_t COARSE_MASK    = 0b00000000111111110000000000000000;
  constexpr uint32_t FINE_MASK      = 0b00000000000000001111111111111111;

  constexpr uint32_t SUB_HITS_MASK   = 0b11111000000000000000000000000000;
  constexpr uint32_t TDC_ERR_MASK    = 0b00000100000000000000000000000000;
  constexpr uint32_t PED_MASK       = 0b00000010000000000000000000000000;
  constexpr uint32_t GAIN_MASK      = 0b00000001000000000000000000000000;
  constexpr uint32_t CHARGE_MASK    = 0b00000000111111111111000000000000;
  constexpr uint32_t STOP_MASK      = 0b00000000000000000000111111111111;

  /*
  constexpr uint32_t TYPE_MASK      = 0xC0000000;
  constexpr uint32_t BOARDNUM_MASK  = 0x20000000;
  constexpr uint32_t IDOD_MASK      = 0x10000000;
  constexpr uint32_t CHANNEL_MASK   = 0x0F000000;
  constexpr uint32_t COARSE_MASK    = 0x00FF0000;
  constexpr uint32_t FINE_MASK      = 0x0000FFFF;

  constexpr uint32_t SUBHITS_MASK   = 0xF8000000;
  constexpr uint32_t TDCERR_MASK    = 0x04000000;
  constexpr uint32_t PED_MASK       = 0x02000000;
  constexpr uint32_t GAIN_MASK      = 0x01000000;
  constexpr uint32_t CHARGE_MASK    = 0x00FFF000;
  constexpr uint32_t STOP_MASK      = 0x00000FFF;
  */
}


RAWIDODHit::RAWIDODHit(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODHit::RAWIDODHit(uint32_t* pointer){
  
  data = pointer;

}

uint8_t RAWIDODHit::GetType() const { return data[0] >> 30;}
bool RAWIDODHit::GetBoardNum() const { return ((data[0] & BOARD_NUM_MASK) >> 29);}
bool RAWIDODHit::GetIDOD() const { return ((data[0] & IDOD_MASK) >> 28);}
uint8_t RAWIDODHit::GetChannel() const {return ((data[0] & CHANNEL_MASK) >> 24);}
uint8_t RAWIDODHit::GetCoarse() const {return ((data[0] & COARSE_MASK) >> 16);}
uint16_t RAWIDODHit::GetFine() const {return (data[0] & FINE_MASK);}
uint8_t RAWIDODHit::GetNumSubHits() const {return ((data[1] & SUB_HITS_MASK) >> 27);}
bool RAWIDODHit::GetTDCError() const {return ((data[1] & TDC_ERR_MASK) >> 26);}
bool RAWIDODHit::GetPed() const {return ((data[1] & PED_MASK) >> 25);}
bool RAWIDODHit::GetGain() const {return ((data[1] & GAIN_MASK) >> 24);}
uint16_t RAWIDODHit::GetCharge() const {return ((data[1] & CHARGE_MASK) >> 12);}
uint16_t RAWIDODHit::GetStop() const {return (data[1] & STOP_MASK);}
size_t RAWIDODHit::GetSize() const { return 64 + ( (GetNumSubHits()>15) ? 480 : (GetNumSubHits() * 32));}
size_t RAWIDODHit::GetWords() const { return 2 + ( (GetNumSubHits()>15) ? 15 : (GetNumSubHits()) );}

void RAWIDODHit::Print() const {
  
  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<std::endl;
  std::cout<<"Type: "<<std::to_string(GetType())<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"BoardNum: "<<GetBoardNum()<<" , "<<std::bitset<1>(GetBoardNum())<<std::endl;
  std::cout<<"IDOD: "<<GetIDOD()<<" , "<<std::bitset<1>(GetIDOD())<<std::endl;
  std::cout<<"Channel: "<<std::to_string(GetChannel())<<" , "<<std::bitset<4>(GetChannel())<<std::endl;
  std::cout<<"Coarse: "<<std::to_string(GetCoarse())<<" , "<<std::bitset<8>(GetCoarse())<<std::endl;
  std::cout<<"Fine: "<<GetFine()<<" , "<<std::bitset<16>(GetFine())<<std::endl;
  std::cout<<"NumSubHits: "<<std::to_string(GetNumSubHits())<<" , "<<std::bitset<5>(GetNumSubHits())<<std::endl;
  std::cout<<"TDCError: "<<GetTDCError()<<" , "<<std::bitset<1>(GetTDCError())<<std::endl;
  std::cout<<"Ped: "<<GetPed()<<" , "<<std::bitset<1>(GetPed())<<std::endl;
  std::cout<<"Gain: "<<GetGain()<<" , "<<std::bitset<1>(GetGain())<<std::endl;
  std::cout<<"Charge: "<<GetCharge()<<" , "<<std::bitset<12>(GetCharge())<<std::endl;
  std::cout<<"Stop: "<<GetStop()<<" , "<<std::bitset<12>(GetStop())<<std::endl;

}



IDODHit::IDODHit(){
  data.push_back(0);
  data.push_back(0);
  SetType(0b11);
}

IDODHit::IDODHit(uint8_t coarse, uint16_t fine){
  
  data.push_back(rand() % UINT_MAX);
  data.push_back(rand() % UINT_MAX);
  SetCoarse(coarse);
  SetFine(fine);
  SetNumSubHits(0);
  SetType(0b11);
}

uint8_t IDODHit::GetType() const { return data.at(0) >> 30;}
bool IDODHit::GetBoardNum() const { return ((data.at(0) & BOARD_NUM_MASK) >> 29);}
bool IDODHit::GetIDOD() const { return ((data.at(0) & IDOD_MASK) >> 28);}
uint8_t IDODHit::GetChannel() const {return ((data.at(0) & CHANNEL_MASK) >> 24);}
uint8_t IDODHit::GetCoarse() const {return ((data.at(0) & COARSE_MASK) >> 16);}
uint16_t IDODHit::GetFine() const {return (data.at(0) & FINE_MASK);}
uint8_t IDODHit::GetNumSubHits() const {return ((data.at(1) & SUB_HITS_MASK) >> 27);}
bool IDODHit::GetTDCError() const {return ((data.at(1) & TDC_ERR_MASK) >> 26);}
bool IDODHit::GetPed() const {return ((data.at(1) & PED_MASK) >> 25);}
bool IDODHit::GetGain() const {return ((data.at(1) & GAIN_MASK) >> 24);}
uint16_t IDODHit::GetCharge() const {return ((data.at(1) & CHARGE_MASK) >> 12);}
uint16_t IDODHit::GetStop() const {return (data.at(1) & STOP_MASK);}
size_t IDODHit::GetSize() const { return 64 + ( (GetNumSubHits()>15) ? 480 : (GetNumSubHits() * 32));}
size_t IDODHit::GetWords() const { return 2 + ( (GetNumSubHits()>15) ? 15 : (GetNumSubHits()) );}
uint32_t* IDODHit::GetSubHits(){ 
  if(GetNumSubHits() == 0) return 0;
  return &data.at(3);
}
std::vector<uint32_t>* IDODHit::GetData(){ return &data;}

void IDODHit::SetType(uint8_t in){ data.at(0) = (data.at(0) & ~TYPE_MASK) | (((uint32_t)in & 0b11) <<30 );} 
void IDODHit::SetBoardNum(bool in){data.at(0) = (data.at(0) & ~BOARD_NUM_MASK) | (((uint32_t)in & 0b1) <<29 );} 
void IDODHit::SetIDOD(bool in){data.at(0) = (data.at(0) & ~IDOD_MASK) | (((uint32_t)in & 0b1) <<28 );}
void IDODHit::SetChannel(uint8_t in){data.at(0) = (data.at(0) & ~CHANNEL_MASK) | (((uint32_t)in & 0b1111) <<24 );}
void IDODHit::SetCoarse(uint8_t in){data.at(0) = (data.at(0) & ~COARSE_MASK) | (((uint32_t)in) <<16 );}
void IDODHit::SetFine(uint16_t in){data.at(0) = (data.at(0) & ~FINE_MASK) | ((uint32_t)in);}
void IDODHit::SetNumSubHits(uint8_t in){
  if(in>15) data.resize(17);  
  else data.resize(2+in);
  data.at(1) = (data.at(1) & ~SUB_HITS_MASK) | (((uint32_t)in & 0b11111) <<27 );
}
void IDODHit::SetTDCError(bool in){data.at(1) = (data.at(1) & ~TDC_ERR_MASK) | (((uint32_t)in & 0b1) <<26 );}
void IDODHit::SetPed(bool in){data.at(1) = (data.at(1) & ~PED_MASK) | (((uint32_t)in & 0b1) <<25 );}
void IDODHit::SetGain(bool in){data.at(1) = (data.at(1) & ~GAIN_MASK) | (((uint32_t)in & 0b1) <<24 );}
void IDODHit::SetCharge(uint16_t in){data.at(1) = (data.at(1) & ~CHARGE_MASK) | (((uint32_t)in & 0b111111111111) <<12 );}
void IDODHit::SetStop(uint16_t in){data.at(1) = (data.at(1) & ~STOP_MASK) | (((uint32_t)in & 0b111111111111) );}
		      
void IDODHit::Print() const {

  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<std::endl;
  std::cout<<"Type: "<<std::to_string(GetType())<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"BoardNum: "<<GetBoardNum()<<" , "<<std::bitset<1>(GetBoardNum())<<std::endl;
  std::cout<<"IDOD: "<<GetIDOD()<<" , "<<std::bitset<1>(GetIDOD())<<std::endl;
  std::cout<<"Channel: "<<std::to_string(GetChannel())<<" , "<<std::bitset<4>(GetChannel())<<std::endl;
  std::cout<<"Coarse: "<<std::to_string(GetCoarse())<<" , "<<std::bitset<8>(GetCoarse())<<std::endl;
  std::cout<<"Fine: "<<GetFine()<<" , "<<std::bitset<16>(GetFine())<<std::endl;
  std::cout<<"NumSubHits: "<<std::to_string(GetNumSubHits())<<" , "<<std::bitset<5>(GetNumSubHits())<<std::endl;
  std::cout<<"TDCError: "<<GetTDCError()<<" , "<<std::bitset<1>(GetTDCError())<<std::endl;
  std::cout<<"Ped: "<<GetPed()<<" , "<<std::bitset<1>(GetPed())<<std::endl;
  std::cout<<"Gain: "<<GetGain()<<" , "<<std::bitset<1>(GetGain())<<std::endl;
  std::cout<<"Charge: "<<GetCharge()<<" , "<<std::bitset<12>(GetCharge())<<std::endl;
  std::cout<<"Stop: "<<GetStop()<<" , "<<std::bitset<12>(GetStop())<<std::endl;
  

}

