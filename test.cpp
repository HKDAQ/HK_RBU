#include <iostream>
#include <RAWIDODHit.h>
#include <RAWIDODSync.h>
#include <bitset>

int main(){

  IDODHit tmp;

  tmp.SetBoardNum(0b1);
  tmp.SetIDOD(1);
  tmp.SetChannel(10);
  tmp.SetCoarse(60);
  tmp.SetFine(7000);
  tmp.SetNumSubHits(7);
  tmp.SetTDCError(true);
  tmp.SetPed(1);
  tmp.SetGain(1);
  tmp.SetCharge(1638);
  tmp.SetStop(543);

  tmp.Print();


  IDODSync tmp2;


  tmp2.SetBoardNum(0b1);
  tmp2.SetIDOD(1);
  tmp2.SetChannel(10);
  tmp2.SetSync500(true);
  tmp2.SetSync125(1);
  tmp2.SetReserved(893724);
  tmp2.SetCounter(890604740);

  tmp2.Print();




  return 0;

}
