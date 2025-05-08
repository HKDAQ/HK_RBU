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
  uint8_t GetType() const;
  bool GetBoardNum() const;
  bool GetIDOD() const;
  uint8_t GetChannel() const;
  uint8_t GetCoarse() const;
  uint16_t GetFine() const;
  uint8_t GetNumSubHits() const;
  bool GetTDCError() const;
  bool GetPed() const;
  bool GetGain() const;
  uint16_t GetCharge() const;
  uint16_t GetStop() const;
  size_t GetSize() const;
  size_t GetWords() const;
  void Print() const;
  
 private:
  
  uint32_t* data;
  
};

class RAWrIDODHit{

 public:

  RAWrIDODHit();
  uint8_t GetType() const;
  bool GetBoardNum() const;
  bool GetIDOD() const;
  uint8_t GetChannel() const;
  uint8_t GetCoarse() const;
  uint16_t GetFine() const;
  uint8_t GetNumSubHits() const;
  bool GetTDCError() const;
  bool GetPed() const;
  bool GetGain() const;
  uint16_t GetCharge() const;
  uint16_t GetStop() const;
  size_t GetSize() const;
  size_t GetWords() const;
  void Print() const;

 private:

  uint32_t data[2];

};

class IDODHit{
  
 public:
  
  IDODHit();
  IDODHit(uint8_t coarse, uint16_t fine);  
  uint8_t GetType() const;
  bool GetBoardNum() const;
  bool GetIDOD() const;
  uint8_t GetChannel() const;
  uint8_t GetCoarse() const;
  uint16_t GetFine() const;
  uint8_t GetNumSubHits() const;
  bool GetTDCError() const;
  bool GetPed() const;
  bool GetGain() const;
  uint16_t GetCharge() const;
  uint16_t GetStop() const;
  size_t GetSize() const;
  size_t GetWords() const;
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

  void Print() const;
  
 private:

  void SetType(uint8_t in);
  
  std::vector<uint32_t> data;
  
};





#endif
