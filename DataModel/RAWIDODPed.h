#ifndef RAW_IDOD_PED_H
#define RAW_IDOD_PED_H

#include <stddef.h>
#include <cstdint>

class RAWIDODPed{
  
 public:
  RAWIDODPed(void* pointer);
  RAWIDODPed(uint32_t* pointer);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  uint32_t GetCoarse();
  static uint32_t GetCoarse(uint32_t* word);
  uint16_t GetReserved();
  bool GetPileup();
  bool GetPed();
  uint16_t GetReserved2();
  uint16_t GetLowCharge();
  uint16_t GetHighCharge();
  static size_t GetSize();
  static size_t GetWords();
  
  
 private:
  
  uint32_t* data;
  
};





#endif
