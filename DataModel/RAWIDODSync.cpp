#include <RAWIDODSync.h>

RAWIDODSync::RAWIDODSync(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODSync::RAWIDODSync(uint32_t* pointer){
  
  data = pointer;

}



uint8_t RAWIDODSync::GetType(){ return data[0] >> 30;}
bool RAWIDODSync::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool RAWIDODSync::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t RAWIDODSync::GetChannel(){return ((data[0] >> 24) & 0b1111);}
bool RAWIDODSync::GetSync500(){return ((data[0] >> 23) & 0b1);}
bool RAWIDODSync::GetSync125(){return ((data[0] >> 22) & 0b1);}
uint32_t RAWIDODSync::GetReserved(){return (data[0] & 0b1111111111111111111111);}
uint32_t RAWIDODSync::GetCounter(){return data[1];}
bool RAWIDODSync::GetSync500(uint32_t* word){return ((word[0] >> 23) & 0b1);}
bool RAWIDODSync::GetSync125(uint32_t* word){return ((word[0] >> 22) & 0b1);}
uint32_t RAWIDODSync::GetCounter(uint32_t* word){return word[1];}
size_t RAWIDODSync::GetSize(){ return 64;}
size_t RAWIDODSync::GetWords(){ return 2;}

void RAWIDODSync::Print(){
  
  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<std::endl;
  std::cout<<"Type: "<<std::to_string(GetType())<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"BoardNum: "<<GetBoardNum()<<" , "<<std::bitset<1>(GetBoardNum())<<std::endl;
  std::cout<<"IDOD: "<<GetIDOD()<<" , "<<std::bitset<1>(GetIDOD())<<std::endl;
  std::cout<<"Channel: "<<std::to_string(GetChannel())<<" , "<<std::bitset<4>(GetChannel())<<std::endl;
  std::cout<<"Sync500: "<<std::to_string(GetSync500())<<" , "<<std::bitset<1>(GetSync500())<<std::endl;
  std::cout<<"Sync125: "<<std::to_string(GetSync125())<<" , "<<std::bitset<1>(GetSync125())<<std::endl;
  std::cout<<"Reserved: "<<std::to_string(GetReserved())<<" , "<<std::bitset<22>(GetReserved())<<std::endl;
  std::cout<<"Counter: "<<std::to_string(GetCounter())<<" , "<<std::bitset<32>(GetCounter())<<std::endl;



}
    

IDODSync::IDODSync(){  
  data[0] = 0;
  data[1] = 0;
  SetType(0b10);
}

IDODSync::IDODSync(uint32_t counter){

  data[0] = rand() % UINT_MAX;  
  data[1] = rand() % UINT_MAX;
  SetCounter(counter);
  SetType(0b10);

}



uint8_t IDODSync::GetType(){ return data[0] >> 30;}
bool IDODSync::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool IDODSync::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t IDODSync::GetChannel(){return ((data[0] >> 24) & 0b1111);}
bool IDODSync::GetSync500(){return ((data[0] >> 23) & 0b1);}
bool IDODSync::GetSync125(){return ((data[0] >> 22) & 0b1);}
uint32_t IDODSync::GetReserved(){return (data[0] & 0b1111111111111111111111);}
uint32_t IDODSync::GetCounter(){return data[1];}
size_t IDODSync::GetSize(){ return 64;}
size_t IDODSync::GetWords(){ return 2;}
uint32_t* IDODSync::GetData(){ return &data[0];}

void IDODSync::SetType(uint8_t in){data[0] = (data[0] & 0b00111111111111111111111111111111) | ((in & 0b11) << 30);}
void IDODSync::SetBoardNum(bool in){ data[0] = (data[0] & 0b11011111111111111111111111111111) | ((in & 0b1) << 29);}
void IDODSync::SetIDOD(bool in){data[0] = (data[0] & 0b11101111111111111111111111111111) | ((in & 0b1) << 28);}
void IDODSync::SetChannel(uint8_t in){data[0] = (data[0] & 0b11110000111111111111111111111111) | ((in & 0b1111) << 24);}
void IDODSync::SetSync500(bool in){data[0] = (data[0] & 0b11111111011111111111111111111111) | ((in & 0b1) << 23);}
void IDODSync::SetSync125(bool in){data[0] = (data[0] & 0b11111111101111111111111111111111) | ((in & 0b1) << 22);}
void IDODSync::SetReserved(uint32_t in){data[0] = (data[0] & 0b11111111110000000000000000000000) | ((in & 0b1111111111111111111111) );}
void IDODSync::SetCounter(uint32_t in){data[1] = in;}

void IDODSync::Print(){
  
  std::cout<<std::bitset<32>(data[0])<<" , "<<std::bitset<32>(data[1])<<std::endl;
  std::cout<<"Type: "<<std::to_string(GetType())<<" , "<<std::bitset<2>(GetType())<<std::endl;
  std::cout<<"BoardNum: "<<GetBoardNum()<<" , "<<std::bitset<1>(GetBoardNum())<<std::endl;
  std::cout<<"IDOD: "<<GetIDOD()<<" , "<<std::bitset<1>(GetIDOD())<<std::endl;
  std::cout<<"Channel: "<<std::to_string(GetChannel())<<" , "<<std::bitset<4>(GetChannel())<<std::endl;
  std::cout<<"Sync500: "<<std::to_string(GetSync500())<<" , "<<std::bitset<1>(GetSync500())<<std::endl;
  std::cout<<"Sync125: "<<std::to_string(GetSync125())<<" , "<<std::bitset<1>(GetSync125())<<std::endl;
  std::cout<<"Reserved: "<<std::to_string(GetReserved())<<" , "<<std::bitset<22>(GetReserved())<<std::endl;
  std::cout<<"Counter: "<<std::to_string(GetCounter())<<" , "<<std::bitset<32>(GetCounter())<<std::endl;

}
