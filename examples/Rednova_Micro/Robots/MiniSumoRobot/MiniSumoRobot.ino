#include <RednovaV2_1.h>

/* 
  Rednova V2 Mini Sumo Robot Example Code
  Hardware Overview:
  - RZ60S opponent sensor x 4
      • 2 at front: LeftFront (A5), RightFront (A3) – front opponent detection
      • 2 at sides: Left (A6), Right (A2) – 90 degree scanning
  - Forceup 625 RPM Motor x 2
  - 11.1V LiPo Battery x 1
  - QTR-1A Line Sensor x 2 (A7 and A1) – line detection
  - Custom Chassis - 450 Gram
  - Bond Silicone Wheels
  - Kanzawa Mini Sumo Blade
  - Startzade-TR, Startzade-RO, Startzade-JP


  Websites:
  - www.robotzade.com
  - www.sumozade.com

  Sensor Placement:
  - Front Sensors: 2 units (LeftFront, RightFront)
  - Side Sensors: 2 units (Left, Right)
  - Line Sensors: 2 units (LQtr, RQtr)
*/

int TurnDeg = 0;      // 1 - Left Turn , 2 - Right Turn
int StartStatus = 0;  // 0 - Stop , 1 - Start
int LeftSensor = A6;      // Left side RZ60 opponent sensor
int LFrontSensor = A5;    // Left front RZ60 opponent sensor
int RFrontSensor = A3;    // Right front RZ60 opponent sensor
int RightSensor = A2;     // Right side RZ60 opponent sensor
int LQtr = A7;            // Left line sensor (QTR-1A)
int RQtr = A1;            // Right line sensor (QTR-1A)
const int samples = 10;     // Number of analog readings
const int lowConfirm = 16;  // Number of consecutive samples to confirm LOW

// Counters and states for each sensor
int LCounter = 0, LFCounter = 0, RFCounter = 0, RCounter = 0, BCounter = 0;
bool LState = 0, LFrontState = 0, RFrontState = 0, RState = 0, BState = 0;

void setup() {
  RednovaV2_1.begin();        // Set In-Out Pins (Switches, Button, Trimpot, Drivers ....)
  RednovaV2_1.PlayStartup();  // Start Code's (Buzzer and RGB LED)
  Serial.begin(9600);

  // Set opponent sensors as input
  pinMode(LeftSensor, INPUT_PULLUP);
  pinMode(LFrontSensor, INPUT_PULLUP);
  pinMode(RFrontSensor, INPUT_PULLUP);
  pinMode(RightSensor, INPUT_PULLUP);
}

void loop() {

Wait:
  RednovaV2_1.ReadButton();  // Read start button
  if (RednovaV2_1.ButtonState == 0) {

    RednovaV2_1.ReadSwitch();  // Read switch state
    String SwitchVal = RednovaV2_1.SwitchState;

    // --- VALID SWITCHES AND MOVEMENTS ---
    if (SwitchVal == "0000") {
      RednovaV2_1.DualDirection(70, 70, 100); // Move forward, slight left turn
      TurnDeg = 1;
    } else if (SwitchVal == "0001") {
      RednovaV2_1.DualDirection(100, -100, 80); // Sharp right turn
      RednovaV2_1.DualDirection(40, 40, 100);
      TurnDeg = 2;
    } else if (SwitchVal == "1000") {
      RednovaV2_1.DualDirection(-100, 100, 80); // Sharp left turn
      RednovaV2_1.DualDirection(40, 40, 100);
      TurnDeg = 1;
    } else if (SwitchVal == "1100") {
      RednovaV2_1.DualDirection(40, 60, 80); // Slight left turn
      RednovaV2_1.DualDirection(-40, 40, 100);
      TurnDeg = 1;
    } else if (SwitchVal == "0011") {
      RednovaV2_1.DualDirection(60, 40, 80); // Slight right turn
      RednovaV2_1.DualDirection(40, -40, 100);
      TurnDeg = 2;
    } else if (SwitchVal == "1110") {
      RednovaV2_1.DualDirection(60, 80, 120); // Fast left turn
      RednovaV2_1.DualDirection(-40, 40, 100);
      TurnDeg = 1;
    } else if (SwitchVal == "0111") {
      RednovaV2_1.DualDirection(80, 60, 120); // Fast right turn
      RednovaV2_1.DualDirection(40, -40, 100);
      TurnDeg = 2;
    } else if (SwitchVal == "1111") {
      RednovaV2_1.DualDirection(70, 70, 100); // Move forward
      TurnDeg = 2;
    } else {
      // Invalid switch combination, default move forward
      RednovaV2_1.DualDirection(100, 100, 10);
      TurnDeg = 0;
    }

    StartStatus = 1;
    goto Start;
  } else {
    StartStatus = 0;
    SensorRead(); // Read sensors
    if (LState == 1 || LFrontState == 1  || RFrontState == HIGH || RState == HIGH) {
      RednovaV2_1.ColorFunction(0, 0, 100); // Opponent detected, LED ON
    } else {
      RednovaV2_1.ColorFunction(0, 0, 0);   // No detection, LED OFF
    }
  }
  goto Wait;

Start:

  RednovaV2_1.ReadButton(); // Check start button
  if (RednovaV2_1.ButtonState == 1) {
    while (1) {
      RednovaV2_1.DualDirection(0, 0, 1); // Stop motors
      RednovaV2_1.MixLed();               // Run LED effects
    }
  }

  // Prevent leaving the line using line sensors
  if (analogRead(LQtr) < 500 && analogRead(RQtr) > 500) {
    RednovaV2_1.DualDirection(-60, -60, 100);
    RednovaV2_1.DualDirection(-60, 60, 110);
    TurnDeg = 0;
  } else if (analogRead(LQtr) > 500 && analogRead(RQtr) < 500) {
    RednovaV2_1.DualDirection(-60, -60, 100);
    RednovaV2_1.DualDirection(60, -60, 110);
    TurnDeg = 0;
  }

  SensorRead(); // Read opponent sensors
  if (LFrontState == 1 && RFrontState == 1) {
    RednovaV2_1.DualDirection(60, 60, 1);
    TurnDeg = 0;
  } else if (LFrontState == 1) {
    RednovaV2_1.DualDirection(40, 60, 1);
    TurnDeg = 1;
  } else if (RFrontState == 1) {
    RednovaV2_1.DualDirection(60, 40, 1);
    TurnDeg = 2;
  } else if ((LFrontState == 1 && LState == 1) || (LFrontState == 0 && LState == 1)) {
    RednovaV2_1.DualDirection(-40, 60, 1);
    TurnDeg = 1;
  } else if ((RFrontState == 1 && RState == 1) || (RFrontState == 0 && RState == 1)) {
    RednovaV2_1.DualDirection(60, -40, 1);
    TurnDeg = 2;
  } else {
    if (TurnDeg == 1) {
      RednovaV2_1.DualDirection(-40, 40, 3);
    } else if (TurnDeg == 2) {
      RednovaV2_1.DualDirection(40, -40, 3);
    } else {
      RednovaV2_1.DualDirection(40, 40, 3);
    }
  }
  goto Start;
}


void SensorRead() {
  int Lsum = 0, LFsum = 0, RFsum = 0, Rsum = 0;

  // ---- Analog reading and filtering ----
  for (int i = 0; i < samples; i++) {
    Lsum += analogRead(LeftSensor);
    LFsum += analogRead(LFrontSensor);
    RFsum += analogRead(RFrontSensor);
    Rsum += analogRead(RightSensor);
    delayMicroseconds(1200);
  }

  // ---- Average value ----
  int LVal = Lsum / samples;
  int LFVal = LFsum / samples;
  int RFVal = RFsum / samples;
  int RVal = Rsum / samples;

  // ---- Digital threshold ----
  int LRaw = (LVal > 30) ? 1 : 0;
  int LFRaw = (LFVal > 30) ? 1 : 0;
  int RFRaw = (RFVal > 30) ? 1 : 0;
  int RRaw = (RVal > 30) ? 1 : 0;

  // ---- LOW confirmation (debounce) ----
  // LeftSensor
  if (LRaw == 0) {
    LCounter++;
    if (LCounter >= lowConfirm) {
      LState = 0;
      LCounter = lowConfirm;
    }
  } else {
    LState = 1;
    LCounter = 0;
  }

  // LFrontSensor
  if (LFRaw == 0) {
    LFCounter++;
    if (LFCounter >= lowConfirm) {
      LFrontState = 0;
      LFCounter = lowConfirm;
    }
  } else {
    LFrontState = 1;
    LFCounter = 0;
  }

  // RFrontSensor
  if (RFRaw == 0) {
    RFCounter++;
    if (RFCounter >= lowConfirm) {
      RFrontState = 0;
      RFCounter = lowConfirm;
    }
  } else {
    RFrontState = 1;
    RFCounter = 0;
  }

  // RightSensor
  if (RRaw == 0) {
    RCounter++;
    if (RCounter >= lowConfirm) {
      RState = 0;
      RCounter = lowConfirm;
    }
  } else {
    RState = 1;
    RCounter = 0;
  }
}
