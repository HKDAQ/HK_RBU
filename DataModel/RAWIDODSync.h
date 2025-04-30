#ifndef RAW_IDOD_SYNC_H
#define RAW_IDOD_SYNC_H

#include <stddef.h>
#include <cstdint>

class RAWIDODSync{
  
 public:
  RAWIDODSync(void* pointer);
  RAWIDODSync(uint32_t* pointer);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  bool GetSync500();
  bool GetSync125();
  uint32_t GetReserved();
  uint32_t GetCounter();
  static uint32_t GetCounter(uint32_t* word);
  static size_t GetSize();
  static size_t GetWords();
  
  
 private:
  
  uint32_t* data;
  
};





#endif
