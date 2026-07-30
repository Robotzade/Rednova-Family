#include <RednovaV2_1.h>

void setup() {
  Serial.begin(9600);
  RednovaV2_1.begin();

  while (!Serial && millis() < 3000) {
  }

  Serial.print(F("Selected board: "));
  Serial.println(RednovaV2_1.BoardName());
}

void loop() {
}
