#include <RAWIDODPed.h>

RAWIDODPed::RAWIDODPed(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODPed::RAWIDODPed(uint32_t* pointer){
  
  data = pointer;

}



uint8_t RAWIDODPed::GetType(){ return data[0] >> 30;}
bool RAWIDODPed::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool RAWIDODPed::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t RAWIDODPed::GetChannel(){return ((data[0] >> 24) & 0b1111);}
uint32_t RAWIDODPed::GetCoarse(){return (data[0] & 0b111111111111111111111111);}
uint32_t RAWIDODPed::GetCoarse(uint32_t* word){return (word[0] & 0b111111111111111111111111);}
uint16_t RAWIDODPed::GetReserved(){return ((data[1] >> 27) & 0b11111);}
bool RAWIDODPed::GetPileup(){return ((data[1] >> 26) & 0b1);}
bool RAWIDODPed::GetPed(){return ((data[1] >> 25) & 0b1);}
uint16_t RAWIDODPed::GetReserved2(){return ((data[1] >> 24) & 0b1);}
uint16_t RAWIDODPed::GetLowCharge(){return ((data[1] >>12) & 0b111111111111);}
uint16_t RAWIDODPed::GetHighCharge(){return (data[1] & 0b111111111111);}
size_t RAWIDODPed::GetSize(){ return 64;}
size_t RAWIDODPed::GetWords(){ return 2;}
    
