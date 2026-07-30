#include <RednovaV2_1.h>

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
  RednovaV2_1.begin();        // Initialize pins
  RednovaV2_1.PlayStartup();  // Startup buzzer + LED effect
}

void loop() {
  // Read inputs
  RednovaV2_1.ReadButton();    // Read button state
  RednovaV2_1.ReadTrimpot();   // Read speed control trimpot

  int speed = map(RednovaV2_1.TrimpotState, 0, 1023, 0, 100); // Map trimpot to 0-100%

  // Detect button press (toggle on rising edge)
  if (RednovaV2_1.ButtonState == 1 && lastButtonState == 0) {
    motorReverse = !motorReverse; // Change motor direction
  }
  lastButtonState = RednovaV2_1.ButtonState;

  // Motor directions based on current toggle state
  float leftMotor = motorReverse ? -speed : speed;
  float rightMotor = motorReverse ? -speed : speed;

  // Apply motor values
  RednovaV2_1.DualDirection(leftMotor, rightMotor, 100);

  // LED feedback
  if(RednovaV2_1.ButtonState == 1) {
    RednovaV2_1.ColorFunction(100, 100, 100); // White  when button pressed
  } else {
    // Normal motor direction LED
    if(motorReverse) RednovaV2_1.ColorFunction(200, 70, 0); // Orange-ish = reverse
    else             RednovaV2_1.ColorFunction(0, 70, 0); // Green = forward
  }

  // Serial feedback
  Serial.print("Speed: "); Serial.print(speed);
  Serial.print(" | Direction: "); Serial.println(motorReverse ? "Reverse" : "Forward");

  delay(100);
}
