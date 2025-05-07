#include <RAWIDODHit.h>

RAWIDODHit::RAWIDODHit(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODHit::RAWIDODHit(uint32_t* pointer){
  
  data = pointer;

}

uint8_t RAWIDODHit::GetType(){ return data[0] >> 30;}
bool RAWIDODHit::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool RAWIDODHit::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t RAWIDODHit::GetChannel(){return ((data[0] >> 24) & 0b1111);}
uint8_t RAWIDODHit::GetCoarse(){return ((data[0] >> 16) & 0b11111111);}
uint16_t RAWIDODHit::GetFine(){return (data[0] & 0b1111111111111111);}
uint8_t RAWIDODHit::GetNumSubHits(){return ((data[1] >> 27) & 0b11111);}
bool RAWIDODHit::GetTDCError(){return ((data[1] >> 26) & 0b1);}
bool RAWIDODHit::GetPed(){return ((data[1] >> 25) & 0b1);}
bool RAWIDODHit::GetGain(){return ((data[1] >> 24) & 0b1);}
uint16_t RAWIDODHit::GetCharge(){return ((data[1] >> 12) & 0b111111111111);}
uint16_t RAWIDODHit::GetStop(){return (data[1] & 0b111111111111);}
size_t RAWIDODHit::GetSize(){ return 64 + ( (GetNumSubHits()>15) ? 480 : (GetNumSubHits() * 32));}
size_t RAWIDODHit::GetWords(){ return 2 + ( (GetNumSubHits()>15) ? 15 : (GetNumSubHits()) );}

void RAWIDODHit::Print(){
  
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

uint8_t IDODHit::GetType(){ return data.at(0) >> 30;}
bool IDODHit::GetBoardNum(){ return ((data.at(0) >> 29) & 0b1);}
bool IDODHit::GetIDOD(){ return ((data.at(0) >> 28) & 0b1);}
uint8_t IDODHit::GetChannel(){return ((data.at(0) >> 24) & 0b1111);}
uint8_t IDODHit::GetCoarse(){return ((data.at(0) >> 16) & 0b11111111);}
uint16_t IDODHit::GetFine(){return (data.at(0) & 0b1111111111111111);}
uint8_t IDODHit::GetNumSubHits(){return ((data.at(1) >> 27) & 0b11111);}
bool IDODHit::GetTDCError(){return ((data.at(1) >> 26) & 0b1);}
bool IDODHit::GetPed(){return ((data.at(1) >> 25) & 0b1);}
bool IDODHit::GetGain(){return ((data.at(1) >> 24) & 0b1);}
uint16_t IDODHit::GetCharge(){return ((data.at(1) >> 12) & 0b111111111111);}
uint16_t IDODHit::GetStop(){return (data.at(1) & 0b111111111111);}
size_t IDODHit::GetSize(){ return 64 + ( (GetNumSubHits()>15) ? 480 : (GetNumSubHits() * 32));}
size_t IDODHit::GetWords(){ return 2 + ( (GetNumSubHits()>15) ? 15 : (GetNumSubHits()) );}
uint32_t* IDODHit::GetSubHits() { 
  if(GetNumSubHits() == 0) return 0;
  return &data.at(3);
}
std::vector<uint32_t>* IDODHit::GetData(){return &data;}

void IDODHit::SetType(uint8_t in){ data.at(0) = (data.at(0) & 0b00111111111111111111111111111111) | (((uint32_t)in & 0b11) <<30 );} 
void IDODHit::SetBoardNum(bool in){data.at(0) = (data.at(0) & 0b11011111111111111111111111111111) | (((uint32_t)in & 0b1) <<29 );} 
void IDODHit::SetIDOD(bool in){data.at(0) = (data.at(0) & 0b11101111111111111111111111111111) | (((uint32_t)in & 0b1) <<28 );}
void IDODHit::SetChannel(uint8_t in){data.at(0) = (data.at(0) & 0b11110000111111111111111111111111) | (((uint32_t)in & 0b1111) <<24 );}
void IDODHit::SetCoarse(uint8_t in){data.at(0) = (data.at(0) & 0b11111111000000001111111111111111) | (((uint32_t)in) <<16 );}
void IDODHit::SetFine(uint16_t in){data.at(0) = (data.at(0) & 0b11111111111111110000000000000000) | ((uint32_t)in);}
void IDODHit::SetNumSubHits(uint8_t in){
  if(in>15) data.resize(17);  
  else data.resize(2+in);
  data.at(1) = (data.at(1) & 0b00000111111111111111111111111111) | (((uint32_t)in & 0b11111) <<27 );
}
void IDODHit::SetTDCError(bool in){data.at(1) = (data.at(1) & 0b11111011111111111111111111111111) | (((uint32_t)in & 0b1) <<26 );}
void IDODHit::SetPed(bool in){data.at(1) = (data.at(1) & 0b11111101111111111111111111111111) | (((uint32_t)in & 0b1) <<25 );}
void IDODHit::SetGain(bool in){data.at(1) = (data.at(1) & 0b11111110111111111111111111111111) | (((uint32_t)in & 0b1) <<24 );}
void IDODHit::SetCharge(uint16_t in){data.at(1) = (data.at(1) & 0b11111111000000000000111111111111) | (((uint32_t)in & 0b111111111111) <<12 );}
void IDODHit::SetStop(uint16_t in){data.at(1) = (data.at(1) & 0b11111111111111111111000000000000) | (((uint32_t)in & 0b111111111111) );}
		      
void IDODHit::Print(){

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

