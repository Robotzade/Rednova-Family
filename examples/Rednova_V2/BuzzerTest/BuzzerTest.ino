#include <RednovaV2_1.h>

// --------------------------------------------------
// Continuous Buzzer Test
// --------------------------------------------------
// This program continuously tests the buzzer on the Rednova V2.1 board.
// The buzzer will sound for 1000ms, then pause for 1000ms, repeating endlessly.
// --------------------------------------------------

void setup() {
  Serial.begin(9600);        // Start serial communication
  RednovaV2_1.begin();       // Initialize Rednova board pins
  RednovaV2_1.PlayStartup(); // Optional: play startup buzzer + LED effect
}

void loop() {
  // Play buzzer for 1000ms
  RednovaV2_1.Buzzer(1, 1000); 
  Serial.println("Buzzer is ON for 1000ms");
  
  // Play buzzer for 1000ms
  RednovaV2_1.Buzzer(0, 1000); 
  Serial.println("Buzzer is OFF for 1000ms");
}
