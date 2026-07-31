#include <Rednova.h>

// --------------------------------------------------
// All Function Test Program
// Switch Test Modes:
// 0000 → Trimpot + Buzzer + Button Test
// 1000 → Forward Motor Test
// 0100 → Left Turn Test
// 0010 → Right Turn Test
// 0001 → Backward Motor Test
// --------------------------------------------------

void setup() {
  Serial.begin(9600);
  Rednova.begin();        // Initialize pin assignments
  Rednova.PlayStartup();  // Play startup buzzer + RGB effect
  //Rednova.StartMelody(); // Optional startup melody
}

void loop() {

  // --------------------------------------------------
  // Read inputs
  // --------------------------------------------------
  Rednova.ReadSwitch();   // Read DIP switch state
  Rednova.ReadButton();   // Read button state
  Rednova.ReadTrimpot();  // Read trimpot value

  String sw = Rednova.SwitchState;
  int button = Rednova.ButtonState;
  int pot = Rednova.TrimpotState;

  // Print input states to serial monitor
  Serial.print("SW:");
  Serial.print(sw);
  Serial.print(" | BTN:");
  Serial.print(button);
  Serial.print(" | POT:");
  Serial.println(pot);



  // --------------------------------------------------
  // 0000 → TRIMPOT + BUZZER + BUTTON TEST
  // --------------------------------------------------
  if (sw == "0000") {
    Rednova.DualDirection(0, 0, 0);  // Stop motors in input-test mode

    // LED color depending on trimpot value
    if (pot < 500) {
      Rednova.ColorFunction(100, 0, 0);  // Red
    } else {
      Rednova.ColorFunction(0, 0, 100);  // Blue
    }
    // --------------------------------------------------
    // PLAY MELODY ONLY WHEN BUTTON IS PRESSED
    // --------------------------------------------------
    if (button == 1) {          // Button pressed
      Rednova.PlayMario();  // Play Mario melody
    }
  }

  // --------------------------------------------------
  // 1000 → FORWARD MOTOR TEST
  // --------------------------------------------------
  else if (sw == "1000") {
    Rednova.ColorFunction(100, 0, 100);  // Magenta
    Rednova.DualDirection(100, 100, 1);  // Forward
  }

  // --------------------------------------------------
  // 0100 → LEFT TURN TEST
  // --------------------------------------------------
  else if (sw == "0100") {
    Rednova.ColorFunction(100, 100, 0);   // Yellow
    Rednova.DualDirection(-100, 100, 1);  // Turn left
  }

  // --------------------------------------------------
  // 0010 → RIGHT TURN TEST
  // --------------------------------------------------
  else if (sw == "0010") {
    Rednova.ColorFunction(0, 100, 100);   // Cyan
    Rednova.DualDirection(100, -100, 1);  // Turn right
  }

  // --------------------------------------------------
  // 0001 → BACKWARD MOTOR TEST
  // --------------------------------------------------
  else if (sw == "0001") {
    Rednova.ColorFunction(100, 100, 100);  // White
    Rednova.DualDirection(-100, -100, 1);  // Backward
  }

  // --------------------------------------------------
  // OTHER (DEFAULT) → LED OFF, MOTORS STOP
  // --------------------------------------------------
  else {
    Rednova.ColorFunction(0, 0, 0);  // LED off
    Rednova.DualDirection(0, 0, 1);  // Motors stop
  }
}
