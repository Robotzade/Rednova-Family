#include <RednovaV2_1.h>

// --------------------------------------------------
// Full RGB LED Test Program
// Cycles through primary, secondary, and mixed colors
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  RednovaV2_1.begin();        // Initialize pins
  RednovaV2_1.PlayStartup();  // Optional startup effect
}

void loop() {
  // ---------- Primary Colors ----------
  RednovaV2_1.ColorFunction(100, 0, 0);  // Red
  delay(500);
  RednovaV2_1.ColorFunction(0, 100, 0);  // Green
  delay(500);
  RednovaV2_1.ColorFunction(0, 0, 100);  // Blue
  delay(500);

  // ---------- Secondary Colors ----------
  RednovaV2_1.ColorFunction(100, 100, 0);  // Yellow
  delay(500);
  RednovaV2_1.ColorFunction(0, 100, 100);  // Cyan
  delay(500);
  RednovaV2_1.ColorFunction(100, 0, 100);  // Magenta
  delay(500);

  // ---------- White ----------
  RednovaV2_1.ColorFunction(100, 100, 100);  // White
  delay(500);




  RednovaV2_1.MixLed();  // Infinite color cycling

  // ---------- Smooth RGB Fade ----------
  for (int i = 0; i <= 255; i += 5) {
    int r = i;        // Red increases
    int g = 255 - i;  // Green decreases
    int b = (i / 2);  // Blue moderate increase
    RednovaV2_1.ColorFunction(r, g, b);
    delay(30);
  }

  // ---------- Fade out ----------
  for (int i = 255; i >= 0; i -= 5) {
    RednovaV2_1.ColorFunction(i, i, i);  // Fade to black
    delay(30);
  }
}
