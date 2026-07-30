#ifndef RednovaV2_1_h // kutuphane ismimiz Rednova
#define RednovaV2_1_h

#include <Arduino.h>

enum RednovaBoardType {
  REDNOVA_V2,
  REDNOVA_MEGA,
  REDNOVA_MICRO
};

class RednovaV2_1Class{ 
  public: 
    RednovaV2_1Class(); 
    void SETUP(); 

void DualDirection(float Lval, float Rval, int Time);    

void Buzzer(boolean x , int Time );
void StartMelody();
void PlayStartup();
void MixLed();
void begin();
void ReadButton();     
void ReadTrimpot();         
void Jump();  
int TrimpotState;         
uint8_t ButtonState;  
void ColorFunction(uint8_t r, uint8_t g, uint8_t b);
void ReadSwitch();    
String SwitchState;
void PlayMario();
RednovaBoardType BoardType() const;
const __FlashStringHelper* BoardName() const;

  private:
    void WriteMotor(uint8_t pwmPin, uint8_t directionPin, float value);
};


extern RednovaV2_1Class RednovaV2_1;

#endif
