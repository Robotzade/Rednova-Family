#include <Rednova.h>

// --------------------------------------------------
// Continuous Mario Melody Test
// Plays Mario melody repeatedly
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  Rednova.begin();        // Initialize pins
  Rednova.PlayStartup();  // Optional startup buzzer + LED effect
}

void loop() {
  Rednova.PlayMario();    // Play Mario melody
  delay(500);                  // Short pause between repetitions
}
