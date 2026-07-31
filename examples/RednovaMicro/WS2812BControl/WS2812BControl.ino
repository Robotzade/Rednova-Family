#include <Rednova.h>

#if !defined(REDNOVA_BOARD_MICRO)
#error "This example requires Tools > Board > Rednova Family > Rednova Micro."
#endif

// --------------------------------------------------
// Full RGB LED Test Program
// Cycles through primary, secondary, and mixed colors
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  Rednova.begin();        // Initialize pins
  Rednova.PlayStartup();  // Optional startup effect
}

void loop() {
  // ---------- Primary Colors ----------
  Rednova.ColorFunction(100, 0, 0);  // Red
  delay(500);
  Rednova.ColorFunction(0, 100, 0);  // Green
  delay(500);
  Rednova.ColorFunction(0, 0, 100);  // Blue
  delay(500);

  // ---------- Secondary Colors ----------
  Rednova.ColorFunction(100, 100, 0);  // Yellow
  delay(500);
  Rednova.ColorFunction(0, 100, 100);  // Cyan
  delay(500);
  Rednova.ColorFunction(100, 0, 100);  // Magenta
  delay(500);

  // ---------- White ----------
  Rednova.ColorFunction(100, 100, 100);  // White
  delay(500);




  Rednova.MixLed();  // Infinite color cycling

  // ---------- Smooth RGB Fade ----------
  for (int i = 0; i <= 255; i += 5) {
    int r = i;        // Red increases
    int g = 255 - i;  // Green decreases
    int b = (i / 2);  // Blue moderate increase
    Rednova.ColorFunction(r, g, b);
    delay(30);
  }

  // ---------- Fade out ----------
  for (int i = 255; i >= 0; i -= 5) {
    Rednova.ColorFunction(i, i, i);  // Fade to black
    delay(30);
  }
}
