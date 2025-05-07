#ifndef RAW_IDOD_HIT_H
#define RAW_IDOD_HIT_H

#include <stddef.h>
#include <cstdint>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <vector>
#include <bitset>
#include <iostream>

class RAWIDODHit{
  
 public:
  
  RAWIDODHit(void* pointer);
  RAWIDODHit(uint32_t* pointer);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  uint8_t GetCoarse();
  uint16_t GetFine();
  uint8_t GetNumSubHits();
  bool GetTDCError();
  bool GetPed();
  bool GetGain();
  uint16_t GetCharge();
  uint16_t GetStop();
  size_t GetSize();
  size_t GetWords();
  void Print();
  
 private:
  
  uint32_t* data;
  
};

class IDODHit{
  
 public:
  
  IDODHit();
  IDODHit(uint8_t coarse, uint16_t fine);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  uint8_t GetCoarse();
  uint16_t GetFine();
  uint8_t GetNumSubHits();
  bool GetTDCError();
  bool GetPed();
  bool GetGain();
  uint16_t GetCharge();
  uint16_t GetStop();
  size_t GetSize();
  size_t GetWords();
  uint32_t* GetSubHits();
  std::vector<uint32_t>* GetData();
  
  void SetBoardNum(bool in);
  void SetIDOD(bool in);
  void SetChannel(uint8_t in);
  void SetCoarse(uint8_t in);
  void SetFine(uint16_t in);
  void SetNumSubHits(uint8_t in);
  void SetTDCError(bool in);
  void SetPed(bool in);
  void SetGain(bool in);
  void SetCharge(uint16_t in);
  void SetStop(uint16_t in);

  void Print();
  
 private:

  void SetType(uint8_t in);
  
  std::vector<uint32_t> data;
  
};





#endif
