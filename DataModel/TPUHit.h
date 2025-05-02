#ifndef TPU_HIT_H
#define TPU_HIT_H

class TPUHit{

 public:

  TPUHit(){;}
  TPUHit(uint8_t type, bool bad_sync, bool gain, uint16_t charge, uint16_t pmt_id, uint32_t time){

    SetType(type);
    SetBadSync(bad_sync);
    SetGain(gain);
    SetCharge(charge);
    SetPMTID(pmt_id);
    SetTime(time);

}

  uint8_t GetType(){return data[0] & 0b11;}
  bool GetBadSync(){return (data[0] >> 2) & 0b1;}
  bool GetGain(){return (data[0] >> 3) & 0b1;}
  uint16_t GetCharge(){return (data[0] >> 4) & 0b111111111111;}
  uint16_t GetPMTID(){return (data[0] >> 16) & 0b1111111111111111;}
  uint32_t GetTime(){ return data[1];}

  void SetType(uint8_t& input){data[0] = (data[0] & 0b11111111111111111111111111111100) | (input & 0b11);}
  void SetBadSync(bool& input){data[0] = (data[0] & 0b11111111111111111111111111111011) | ((input & 0b1) << 2);}
  void SetGain(bool& input){data[0] = (data[0] & 0b11111111111111111111111111110111) | ((input & 0b1) << 3);}
  void SetCharge(uint16_t& input){data[0] = (data[0] & 0b11111111111111110000000000001111) | (( ((uint32_t)input) & 0b111111111111) << 4);}
  void SetPMTID(uint16_t& input){data[0] = (data[0] & 0b00000000000000001111111111111111) | ( ((uint32_t)input) <<16);}
  void SetTime(uint32_t& input){data[1] = input;}
  
 private:
  
  uint32_t data[2]; 
    
};



#endif
