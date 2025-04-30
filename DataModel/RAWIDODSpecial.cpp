#include <RAWIDODSpecial.h>

RAWIDODSpecial::RAWIDODSpecial(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODSpecial::RAWIDODSpecial(uint32_t* pointer){
  
  data = pointer;

}



uint8_t RAWIDODSpecial::GetType(){ return data[0] >> 30;}
bool RAWIDODSpecial::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
uint8_t RAWIDODSpecial::GetReserved(){return ((data[0] >> 24) & 0b11111);}
uint8_t RAWIDODSpecial::GetSpecialPacket(){return ((data[0] >> 16) & 0b11111111);}
uint16_t RAWIDODSpecial::GetReserved2(){return (data[0] & 0b1111111111111111);}
uint32_t RAWIDODSpecial::GetCoarse(){return data[1];}
uint32_t RAWIDODSpecial::GetCoarse(uint32_t* word){return word[1];}
size_t RAWIDODSpecial::GetSize(){ return 64;}
size_t RAWIDODSpecial::GetWords(){ return 2;}
    
