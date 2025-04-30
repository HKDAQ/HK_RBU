#include <RAWIDODHit.h>

RAWIDODHit::RAWIDODHit(void* pointer){
  
  data = reinterpret_cast<uint32_t*>(pointer);

}
RAWIDODHit::RAWIDODHit(uint32_t* pointer){
  
  data = pointer;

}



uint8_t RAWIDODHit::GetType(){ return data[0] >> 30;}
bool RAWIDODHit::GetBoardNum(){ return ((data[0] >> 29) & 0b1);}
bool RAWIDODHit::GetIDOD(){ return ((data[0] >> 28) & 0b1);}
uint8_t RAWIDODHit::GetChannel(){return ((data[0] >> 24) & 0b1111);}
uint8_t RAWIDODHit::GetCoarse(){return ((data[0] >> 16) & 0b11111111);}
uint16_t RAWIDODHit::GetFine(){return (data[0] & 0b1111111111111111);}
uint8_t RAWIDODHit::GetNumSubHits(){return ((data[1] >> 27) & 0b11111);}
bool RAWIDODHit::GetTDCError(){return ((data[1] >> 26) & 0b1);}
bool RAWIDODHit::GetPed(){return ((data[1] >> 25) & 0b1);}
bool RAWIDODHit::GetGain(){return ((data[1] >> 24) & 0b1);}
uint16_t RAWIDODHit::GetCharge(){return ((data[1] >> 12) & 0b111111111111);}
uint16_t RAWIDODHit::GetStop(){return (data[1] & 0b111111111111);}
size_t RAWIDODHit::GetSize(){ return 64 + ( (GetNumSubHits()>15) ? 480 : (GetNumSubHits() * 32));}
size_t RAWIDODHit::GetWords(){ return 2 + ( (GetNumSubHits()>15) ? 15 : (GetNumSubHits()) );}
    
/*
  void RAWIDODHit::SetType(){
  void RAWIDODHit::SetBoardNum(){
  void RAWIDODHit::SetIDOD(){
  void RAWIDODHit::SetChannel(){
  void RAWIDODHit::SetCoarse(){
  void RAWIDODHit::SetFine(){
  void RAWIDODHit::SetNumSubHits(){
  void RAWIDODHit::SetTDCError(){
  void RAWIDODHit::SetPed(){
  void RAWIDODHit::SetCharge(){
  void RAWIDODHit::SetStop(){
*/
