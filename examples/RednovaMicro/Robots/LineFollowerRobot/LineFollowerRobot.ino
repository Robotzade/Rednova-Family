#include <Rednova.h>

#if !defined(REDNOVA_BOARD_MICRO)
#error "This example requires a Rednova Micro board selection, including its Old Boot Leonardo option."
#endif

/*
  Rednova Micro White Line Following Robot with PID
  Hardware Overview:
  - QTR-8A Analog Line Sensor x 8 (A0-A7)
  - Forceup 2000 RPM Motor x 2
  - 11.1V LiPo Battery x 1
  - Bond Silicone Wheels
*/

const int sensorsCount = 8;
int sensorPins[sensorsCount] = { A0, A1, A2, A3, A4, A5, A6, A7 };
int sensorMin[sensorsCount], sensorMax[sensorsCount];  // calibration

// PID parameters
float Kp = 0.5;
float Ki = 0.0;
float Kd = 0.2;
float lastError = 0;
float integral = 0;

void setup() {
  Rednova.begin();
  Serial.begin(9600);
  Rednova.PlayStartup();

  for (int i = 0; i < sensorsCount; i++) {
    pinMode(sensorPins[i], INPUT);
    sensorMin[i] = 1023;
    sensorMax[i] = 0;
  }

  calibrateSensors();  // calibrate min/max values
  delay(500);

  // Butona basılmasını bekle ve 5 bip
  while (Rednova.ButtonState == 0) {
    Rednova.ReadButton();
  }
  beepStart(5);  // 5 beeps
}

void loop() {
  long positionSum = 0;
  int activeCount = 0;

  // Read calibrated sensor values
  for (int i = 0; i < sensorsCount; i++) {
    int val = analogRead(sensorPins[i]);
    if (sensorMax[i] > sensorMin[i]) {
      val = constrain(val, sensorMin[i], sensorMax[i]);
      val = map(val, sensorMin[i], sensorMax[i], 0, 1000);
    } else {
      val = 0;
    }
    int detected = val > 500 ? 1 : 0;  // white line detected
    positionSum += detected * i * 100;
    activeCount += detected;
  }

  int position = 350;
  if (activeCount > 0) {
    position = positionSum / activeCount;
  }

  // PID control
  float error = position - 350;
  integral += error;
  float derivative = error - lastError;
  float turn = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  int baseSpeed = 80;
  int leftSpeed = baseSpeed - turn;
  int rightSpeed = baseSpeed + turn;

  // Limit speed
  leftSpeed = constrain(leftSpeed, -100, 100);
  rightSpeed = constrain(rightSpeed, -100, 100);

  Rednova.DualDirection(leftSpeed, rightSpeed, 1);

}

// ----- Sensor Calibration -----
void calibrateSensors() {
  Serial.println("Calibrating sensors, move robot over line...");
  long start = millis();
  while (millis() - start < 3000) {  // 3 sec calibration
    for (int i = 0; i < sensorsCount; i++) {
      int val = analogRead(sensorPins[i]);
      if (val < sensorMin[i]) sensorMin[i] = val;
      if (val > sensorMax[i]) sensorMax[i] = val;
    }
    delay(20);
  }
  Serial.println("Calibration done");
}

// ----- Beep Function -----
void beepStart(int count) {
  for (int i = 0; i < count; i++) {
    Rednova.Buzzer(1, 200);  // 200ms beep
    Rednova.Buzzer(0, 200);  // 200ms beep
  }
}
