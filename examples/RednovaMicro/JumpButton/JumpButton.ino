#include <Rednova.h>

#if !defined(REDNOVA_BOARD_MICRO)
#error "This example requires a Rednova Micro board selection, including its Old Boot Leonardo option."
#endif

// --------------------------------------------------
// Color Cycle with Jump Effect
// --------------------------------------------------
// This program allows you to cycle through a set of predefined LED colors
// on the Rednova Micro board by pressing the button. Each time the button
// is pressed, the LED changes to the next color in the list, and the
// Rednova.Jump() function is called to play a short jump sound effect.
// --------------------------------------------------

// ---------------- Color Palette -----------------
// Array of RGB values for the colors
uint8_t colors[][3] = {
  {100, 0, 0},     // Red
  {0, 100, 0},     // Green
  {0, 0, 100},     // Blue
  {100, 100, 0},   // Yellow
  {100, 0, 100},   // Magenta
  {0, 100, 100},   // Cyan
  {100, 50, 0},    // Orange
  {50, 0, 100},    // Purple
  {50, 100, 50},   // Light Green
  {100, 100, 100}, // White
};

// Variables to track button state and current color
bool lastButtonState = 0;
int colorIndex = -1;

void setup() {
  Serial.begin(9600);         // Start serial communication
  Rednova.begin();       // Initialize Rednova Micro board
  Rednova.PlayStartup(); // Play startup buzzer + LED effect
}

void loop() {
  Rednova.ReadButton();  // Read current button state

  // Detect button press (rising edge)
  if (Rednova.ButtonState == 1 && lastButtonState == 0) {
    // Increment color index to move to the next color
    colorIndex++;
    if (colorIndex >= (int)(sizeof(colors) / sizeof(colors[0]))) colorIndex = 0;

    // Set LED to the selected color
    Rednova.ColorFunction(colors[colorIndex][0],
                              colors[colorIndex][1],
                              colors[colorIndex][2]);

    // Play the Jump sound effect
    Rednova.Jump();
  }

  // Save current button state for next loop iteration
  lastButtonState = Rednova.ButtonState;
}
