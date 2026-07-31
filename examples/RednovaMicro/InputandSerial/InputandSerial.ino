#include <Rednova.h>

// --------------------------------------------------
// Input Monitor Test Program
// Reads the two DIP switches on D15 and D16, button, and trimpot
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
  Serial.print("Switches (SW1/SW2): ");
  Serial.print(switchState);
  Serial.print(" | Button: ");
  Serial.print(buttonState);
  Serial.print(" | Trimpot: ");
  Serial.println(trimpotValue);

  // Update every 200ms
  delay(200);
}
