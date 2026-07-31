#include <Rednova.h>

#if !defined(REDNOVA_BOARD_V2)
#error "This example requires Tools > Board > Rednova Family > Rednova V2."
#endif

// --------------------------------------------------
// Motor Control Test Program
// Trimpot adjusts motor speed
// Button toggles motor direction (direction stays after release)
// LED turns white  when button is pressed
// --------------------------------------------------

bool motorReverse = false;     // Stores current motor direction
bool lastButtonState = 0;      // For detecting button press change

void setup() {
  Serial.begin(9600);
  Rednova.begin();        // Initialize pins
  Rednova.PlayStartup();  // Startup buzzer + LED effect
}

void loop() {
  // Read inputs
  Rednova.ReadButton();    // Read button state
  Rednova.ReadTrimpot();   // Read speed control trimpot

  int speed = map(Rednova.TrimpotState, 0, 1023, 0, 100); // Map trimpot to 0-100%

  // Detect button press (toggle on rising edge)
  if (Rednova.ButtonState == 1 && lastButtonState == 0) {
    motorReverse = !motorReverse; // Change motor direction
  }
  lastButtonState = Rednova.ButtonState;

  // Motor directions based on current toggle state
  float leftMotor = motorReverse ? -speed : speed;
  float rightMotor = motorReverse ? -speed : speed;

  // Apply motor values
  Rednova.DualDirection(leftMotor, rightMotor, 100);

  // LED feedback
  if(Rednova.ButtonState == 1) {
    Rednova.ColorFunction(100, 100, 100); // White  when button pressed
  } else {
    // Normal motor direction LED
    if(motorReverse) Rednova.ColorFunction(100, 70, 0); // Orange = reverse
    else             Rednova.ColorFunction(0, 70, 0); // Green = forward
  }

  // Serial feedback
  Serial.print("Speed: "); Serial.print(speed);
  Serial.print(" | Direction: "); Serial.println(motorReverse ? "Reverse" : "Forward");

  delay(100);
}
