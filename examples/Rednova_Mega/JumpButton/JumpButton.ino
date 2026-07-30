#include <RednovaV2_1.h>

// --------------------------------------------------
// Color Cycle with Jump Effect
// --------------------------------------------------
// This program allows you to cycle through a set of predefined LED colors
// on the Rednova V2.1 board by pressing the button. Each time the button
// is pressed, the LED changes to the next color in the list, and the
// RednovaV2_1.Jump() function is called to play a short jump sound effect.
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
int colorIndex = 0;

void setup() {
  Serial.begin(9600);         // Start serial communication
  RednovaV2_1.begin();       // Initialize Rednova V2.1 board
  RednovaV2_1.PlayStartup(); // Play startup buzzer + LED effect
}

void loop() {
  RednovaV2_1.ReadButton();  // Read current button state

  // Detect button press (rising edge)
  if (RednovaV2_1.ButtonState == 1 && lastButtonState == 0) {
    // Increment color index to move to the next color
    colorIndex++;
    if (colorIndex >= sizeof(colors)/sizeof(colors[0])) colorIndex = 0;

    // Set LED to the selected color
    RednovaV2_1.ColorFunction(colors[colorIndex][0],
                              colors[colorIndex][1],
                              colors[colorIndex][2]);

    // Play the Jump sound effect
    RednovaV2_1.Jump();
  }

  // Save current button state for next loop iteration
  lastButtonState = RednovaV2_1.ButtonState;
}
