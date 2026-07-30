#include <RednovaV2_1.h>

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
  RednovaV2_1.begin();        // Initialize pin assignments
  RednovaV2_1.PlayStartup();  // Play startup buzzer + RGB effect
  //RednovaV2_1.StartMelody(); // Optional startup melody
}

void loop() {

  // --------------------------------------------------
  // Read inputs
  // --------------------------------------------------
  RednovaV2_1.ReadSwitch();   // Read DIP switch state
  RednovaV2_1.ReadButton();   // Read button state
  RednovaV2_1.ReadTrimpot();  // Read trimpot value

  String sw = RednovaV2_1.SwitchState;
  int button = RednovaV2_1.ButtonState;
  int pot = RednovaV2_1.TrimpotState;

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
    // LED color depending on trimpot value
    if (pot < 500) {
      RednovaV2_1.ColorFunction(100, 0, 0);  // Red
    } else {
      RednovaV2_1.ColorFunction(0, 0, 100);  // Blue
    }
    // --------------------------------------------------
    // PLAY MELODY ONLY WHEN BUTTON IS PRESSED
    // --------------------------------------------------
    if (button == 1) {          // Button pressed
      RednovaV2_1.PlayMario();  // Play Mario melody
    }
  }

  // --------------------------------------------------
  // 1000 → FORWARD MOTOR TEST
  // --------------------------------------------------
  else if (sw == "1000") {
    RednovaV2_1.ColorFunction(100, 0, 100);  // Magenta
    RednovaV2_1.DualDirection(100, 100, 1);  // Forward
  }

  // --------------------------------------------------
  // 0100 → LEFT TURN TEST
  // --------------------------------------------------
  else if (sw == "0100") {
    RednovaV2_1.ColorFunction(100, 100, 0);   // Yellow
    RednovaV2_1.DualDirection(-100, 100, 1);  // Turn left
  }

  // --------------------------------------------------
  // 0010 → RIGHT TURN TEST
  // --------------------------------------------------
  else if (sw == "0010") {
    RednovaV2_1.ColorFunction(0, 100, 100);   // Cyan
    RednovaV2_1.DualDirection(100, -100, 1);  // Turn right
  }

  // --------------------------------------------------
  // 0001 → BACKWARD MOTOR TEST
  // --------------------------------------------------
  else if (sw == "0001") {
    RednovaV2_1.ColorFunction(100, 100, 100);  // White
    RednovaV2_1.DualDirection(-100, -100, 1);  // Backward
  }

  // --------------------------------------------------
  // OTHER (DEFAULT) → LED OFF, MOTORS STOP
  // --------------------------------------------------
  else {
    RednovaV2_1.ColorFunction(0, 0, 0);  // LED off
    RednovaV2_1.DualDirection(0, 0, 1);  // Motors stop
  }
}
