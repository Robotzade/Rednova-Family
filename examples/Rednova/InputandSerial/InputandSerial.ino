#include <Rednova.h>

// --------------------------------------------------
// Input Monitor Test Program
// Continuously reads DIP switches, button, and trimpot
// Displays their values on the Serial Monitor
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  Rednova.begin();       // Initialize pins
  Rednova.PlayStartup(); // Optional startup effect
}

void loop() {
  // Read inputs
  Rednova.ReadSwitch();   // Read DIP switch state
  Rednova.ReadButton();   // Read button state
  Rednova.ReadTrimpot();  // Read trimpot value

  // Get values
  String switchState = Rednova.SwitchState;
  int buttonState = Rednova.ButtonState;
  int trimpotValue = Rednova.TrimpotState;

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
