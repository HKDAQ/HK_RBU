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
uint32_t RAWIDODSync::GetCounter(uint32_t* word){return word[1];}
size_t RAWIDODSync::GetSize(){ return 64;}
size_t RAWIDODSync::GetWords(){ return 2;}
    
