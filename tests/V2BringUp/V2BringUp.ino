// Rednova V2 initial Arduino IDE compatibility test.
// This sketch intentionally does not depend on the Rednova library.

const uint8_t TEST_LED_PIN = LED_BUILTIN;

void setup() {
  pinMode(TEST_LED_PIN, OUTPUT);
  Serial.begin(9600);

  while (!Serial && millis() < 3000) {
    // Allow a short time for the native USB serial connection.
  }

  Serial.println(F("Rednova V2 ready"));
}

void loop() {
  digitalWrite(TEST_LED_PIN, HIGH);
  Serial.println(F("Rednova V2 LED ON"));
  delay(500);

  digitalWrite(TEST_LED_PIN, LOW);
  Serial.println(F("Rednova V2 LED OFF"));
  delay(500);
}
