#ifndef TIME_SLICE_H
#define TIME_SLICE_H

#include <TPUHit.h>

struct TimeSlice{
  
  uint64_t time;
  std::vector<uint32_t> raw_idod;
  std::mutex raw_idod_mtx;
  std::vector<uint32_t> raw_mpmt;
  std::mutex raw_mpmt_mtx;
  std::vector<TPUHit> reduced_hits;
  std::mutex reduced_hits_mtx;

};


#endif
