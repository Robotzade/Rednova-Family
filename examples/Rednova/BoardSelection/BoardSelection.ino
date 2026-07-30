#include <Rednova.h>

void setup() {
  Serial.begin(9600);
  Rednova.begin();

  while (!Serial && millis() < 3000) {
  }

  Serial.print(F("Selected board: "));
  Serial.println(Rednova.BoardName());
}

void loop() {
}
