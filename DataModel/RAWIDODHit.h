#ifndef RAW_IDOD_HIT_H
#define RAW_IDOD_HIT_H

#include <stddef.h>
#include <cstdint>

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
  
  /*
    void SetType();
    void SetBoardNum();
    void SetIDOD();
    void SetChannel();
    void SetCoarse();
    void SetFine();
    void SetNumSubHits();
    void SetTDCError();
    void SetPed();
    void SetCharge();
    void SetStop();
  */
  
 private:
  
  uint32_t* data;
  
};





#endif
