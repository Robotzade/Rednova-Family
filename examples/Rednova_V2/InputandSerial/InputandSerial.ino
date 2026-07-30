#include <RednovaV2_1.h>

// --------------------------------------------------
// Input Monitor Test Program
// Continuously reads DIP switches, button, and trimpot
// Displays their values on the Serial Monitor
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  RednovaV2_1.begin();       // Initialize pins
  RednovaV2_1.PlayStartup(); // Optional startup effect
}

void loop() {
  // Read inputs
  RednovaV2_1.ReadSwitch();   // Read DIP switch state
  RednovaV2_1.ReadButton();   // Read button state
  RednovaV2_1.ReadTrimpot();  // Read trimpot value

  // Get values
  String switchState = RednovaV2_1.SwitchState;
  int buttonState = RednovaV2_1.ButtonState;
  int trimpotValue = RednovaV2_1.TrimpotState;

  // Print to Serial Monitor
  Serial.print("Switches: ");
  Serial.print(switchState);
  Serial.print(" | Button: ");
  Serial.print(buttonState);
  Serial.print(" | Trimpot: ");
  Serial.println(trimpotValue);

  // Update every 200ms
  delay(200);
}
