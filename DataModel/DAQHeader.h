#ifndef DAQ_HEADER_H
#define DAQ_DEADER_H

#include <stddef.h>
#include <cstdint>

class DAQHeader{

 public:

  DAQHeader();
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
