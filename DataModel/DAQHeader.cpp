#include <DAQHeader.h>

DAQHeader::DAQHeader(){;}

uint32_t DAQHeader::GetMessageNum(){ return data[0]; }

uint32_t DAQHeader::GetCoarseCounter(){ return data[1]; }

uint8_t DAQHeader::GetType(){ return data[2] >> 30;}

uint16_t DAQHeader::GetCardID(){ return (data[2] >> 18) & 0b111111111111;}

uint16_t DAQHeader::GetNumWords(){ return (data[2] >> 3) & 0b111111111111111; }

uint8_t DAQHeader::GetFlags(){ return data[2] & 0b111; }
