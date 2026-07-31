#include <Rednova.h>

#if !defined(REDNOVA_BOARD_V2) && !defined(ARDUINO_AVR_LEONARDO)
#error "This example requires Rednova V2 or Arduino Leonardo board selection."
#endif

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
