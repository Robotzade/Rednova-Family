#include <Rednova.h>

#if !defined(REDNOVA_BOARD_MICRO)
#error "This example requires the Rednova Micro board selection."
#endif

// Rednova Micro complete hardware test
// SW1 is connected to D15 and SW2 is connected to D16.
//
// 00 -> Trimpot, RGB LED, buzzer and button test
// 10 -> Forward motor test
// 01 -> Backward motor test
// 11 -> Turn-in-place motor test

void setup() {
  Serial.begin(9600);
  Rednova.begin();
  Rednova.PlayStartup();
}

void loop() {
  Rednova.ReadSwitch();
  Rednova.ReadButton();
  Rednova.ReadTrimpot();

  String sw = Rednova.SwitchState;

  Serial.print("SW:");
  Serial.print(sw);
  Serial.print(" | BTN:");
  Serial.print(Rednova.ButtonState);
  Serial.print(" | POT:");
  Serial.println(Rednova.TrimpotState);

  if (sw == "00") {
    Rednova.DualDirection(0, 0, 0);

    if (Rednova.TrimpotState < 500) {
      Rednova.ColorFunction(100, 0, 0);
    } else {
      Rednova.ColorFunction(0, 0, 100);
    }

    if (Rednova.ButtonState == 1) {
      Rednova.PlayMario();
    }
  } else if (sw == "10") {
    Rednova.ColorFunction(100, 0, 100);
    Rednova.DualDirection(100, 100, 1);
  } else if (sw == "01") {
    Rednova.ColorFunction(100, 100, 100);
    Rednova.DualDirection(-100, -100, 1);
  } else if (sw == "11") {
    Rednova.ColorFunction(0, 100, 100);
    Rednova.DualDirection(100, -100, 1);
  } else {
    Rednova.ColorFunction(0, 0, 0);
    Rednova.DualDirection(0, 0, 1);
  }
}
