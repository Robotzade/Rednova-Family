#include <Rednova.h>

#if !defined(REDNOVA_BOARD_MICRO) && !defined(ARDUINO_AVR_LEONARDO)
#error "This example requires Rednova Micro or Arduino Leonardo board selection."
#endif

// --------------------------------------------------
// Continuous Buzzer Test
// --------------------------------------------------
// This program continuously tests the buzzer on the Rednova Micro board.
// The buzzer will sound for 1000ms, then pause for 1000ms, repeating endlessly.
// --------------------------------------------------

void setup() {
  Serial.begin(9600);        // Start serial communication
  Rednova.begin();       // Initialize Rednova board pins
  Rednova.PlayStartup(); // Optional: play startup buzzer + LED effect
}

void loop() {
  // Play the buzzer for 1000ms
  Rednova.Buzzer(1, 1000);
  Serial.println("Buzzer is ON for 1000ms");

  // Stop the buzzer for 1000ms
  Rednova.Buzzer(0, 1000);
  Serial.println("Buzzer is OFF for 1000ms");
}
