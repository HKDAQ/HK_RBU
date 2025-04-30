#include <RAWIDODError.h>

RAWIDODError::RAWIDODError(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODError::RAWIDODError(uint32_t* pointer){
  
  data = pointer;

}



uint8_t RAWIDODError::GetType(){ return data[0] >> 30;}
bool RAWIDODError::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool RAWIDODError::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t RAWIDODError::GetChannel(){return ((data[0] >> 24) & 0b1111);}
bool RAWIDODError::GetTDCError(){return ((data[0] >> 23) & 0b1);}
bool RAWIDODError::GetOutSyncError(){return ((data[0] >> 22) & 0b1);}
bool RAWIDODError::GetError125(){return ((data[0] >> 21) & 0b1);}
bool RAWIDODError::GetError500(){return ((data[0] >> 20) & 0b1);}
uint32_t RAWIDODError::GetReserved(){return (data[0]  & 0b11111111111111111111);}
uint32_t RAWIDODError::GetCoarse(){return data[1];}
uint32_t RAWIDODError::GetCoarse(uint32_t* word){return word[1];}
size_t RAWIDODError::GetSize(){ return 64;}
size_t RAWIDODError::GetWords(){ return 2;}
    
