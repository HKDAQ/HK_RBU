#ifndef RAW_IDOD_SPECIAL_H
#define RAW_IDOD_SPECIAL_H

#include <stddef.h>
#include <cstdint>

class RAWIDODSpecial{
  
 public:
  RAWIDODSpecial(void* pointer);
  RAWIDODSpecial(uint32_t* pointer);  
  uint8_t GetType();
  bool GetBoardNum();
  uint8_t GetReserved();
  uint8_t GetSpecialPacket();
  uint16_t GetReserved2();
  uint32_t GetCoarse();
  static uint32_t GetCoarse(uint32_t* word);
  static size_t GetSize();
  static size_t GetWords();
  
  
 private:
  
  uint32_t* data;
  
};





#endif
