#ifndef Rednova_h // kutuphane ismimiz Rednova
#define Rednova_h

#include <Arduino.h>

enum RednovaBoardType {
  REDNOVA_V2,
  REDNOVA_MEGA,
  REDNOVA_MICRO
};

class RednovaClass{ 
  public: 
    RednovaClass(); 
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


extern RednovaClass Rednova;

#endif
