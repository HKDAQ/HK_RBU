#ifndef RAW_DAQ_HEADER_H
#define RAW_DAQ_HEADER_H

#include <stddef.h>
#include <cstdint>

class RAWDAQHeader{

 public:

  RAWDAQHeader();
  RAWDAQHeader(uint32_t* in_data);
  uint32_t GetMessageNum();
  uint32_t GetCoarseCounter();
  uint8_t GetType();
  uint16_t GetCardID();
  uint16_t GetNumWords();
  uint8_t GetFlags();

 private:

  uint32_t* data;


};

#endif
