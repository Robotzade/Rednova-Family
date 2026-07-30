#include <RednovaV2_1.h>

// --------------------------------------------------
// Continuous Mario Melody Test
// Plays Mario melody repeatedly
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  RednovaV2_1.begin();        // Initialize pins
  RednovaV2_1.PlayStartup();  // Optional startup buzzer + LED effect
}

void loop() {
  RednovaV2_1.PlayMario();    // Play Mario melody
  delay(500);                  // Short pause between repetitions
}
