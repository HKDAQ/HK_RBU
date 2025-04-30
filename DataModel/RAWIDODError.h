#ifndef RAW_IDOD_ERROR_H
#define RAW_IDOD_ERROR_H

#include <stddef.h>
#include <cstdint>

class RAWIDODError{
  
 public:
  RAWIDODError(void* pointer);
  RAWIDODError(uint32_t* pointer);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  bool GetTDCError();
  bool GetOutSyncError();
  bool GetError125();
  bool GetError500();
  uint32_t GetReserved();
  uint32_t GetCoarse();
  static uint32_t GetCoarse(uint32_t* word);
  static size_t GetSize();
  static size_t GetWords();
 
  
 private:
  
  uint32_t* data;
  
};





#endif
