#include <EEPROM.h>

// Factory/service tool: permanently identifies the connected ATmega32U4
// controller as Rednova Micro. Upload with Tools > Board > Arduino Leonardo.
// Normal Rednova uploads preserve these final eight EEPROM bytes.

const int IDENTITY_ADDRESS = E2END - 7;
const byte MICRO_IDENTITY[8] = {
  0x52, 0x44, 0x4E, 0x56,  // "RDNV"
  0x01,                    // Identity format version
  0x03,                    // Rednova Micro
  0xFC,                    // Model integrity byte
  0xA5                     // Identity terminator
};

bool identityMatches() {
  for (byte index = 0; index < sizeof(MICRO_IDENTITY); index++) {
    if (EEPROM.read(IDENTITY_ADDRESS + index) != MICRO_IDENTITY[index]) {
      return false;
    }
  }
  return true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);

  for (byte index = 0; index < sizeof(MICRO_IDENTITY); index++) {
    EEPROM.update(IDENTITY_ADDRESS + index, MICRO_IDENTITY[index]);
  }

  if (identityMatches()) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Rednova Micro factory identity written successfully.");
    Serial.println("Operation completed. Select the Rednova Micro board for future uploads.");
  } else {
    Serial.println("ERROR: Rednova Micro factory identity verification failed.");
  }
}

void loop() {}
