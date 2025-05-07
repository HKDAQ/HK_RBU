#ifndef RAW_IDOD_SYNC_H
#define RAW_IDOD_SYNC_H

#include <stddef.h>
#include <cstdint>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <limits.h>
#include <vector>
#include <bitset>
#include <iostream>

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
  static bool GetSync500(uint32_t* word);
  static bool GetSync125(uint32_t* word);
  static uint32_t GetCounter(uint32_t* word);
  static size_t GetSize();
  static size_t GetWords();
  void Print();  
  
 private:
  
  uint32_t* data;
  
};

class IDODSync{
  
 public:
  IDODSync();
  IDODSync(uint32_t counter);  
  uint8_t GetType();
  bool GetBoardNum();
  bool GetIDOD();
  uint8_t GetChannel();
  bool GetSync500();
  bool GetSync125();
  uint32_t GetReserved();
  uint32_t GetCounter();
  static size_t GetSize();
  static size_t GetWords();
  uint32_t* GetData();
 
  void SetBoardNum(bool in);
  void SetIDOD(bool in);
  void SetChannel(uint8_t in);
  void SetSync500(bool in);
  void SetSync125(bool in);
  void SetReserved(uint32_t in);
  void SetCounter(uint32_t in);
  void Print();
  
 private:

  void SetType(uint8_t in);
  
  uint32_t data[2];
  
};





#endif
