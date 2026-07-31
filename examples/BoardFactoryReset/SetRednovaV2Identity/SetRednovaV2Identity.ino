#include <EEPROM.h>

// Factory/service tool: permanently identifies the connected ATmega32U4
// controller as Rednova V2. Upload with Tools > Board > Arduino Leonardo.
// Normal Rednova uploads preserve these final eight EEPROM bytes.

const int IDENTITY_ADDRESS = E2END - 7;
const byte V2_IDENTITY[8] = {
  0x52, 0x44, 0x4E, 0x56,  // "RDNV"
  0x02,                    // Identity format version
  0x02,                    // Rednova V2
  0x46,                    // Flexible Leonardo policy
  0xA5                     // Identity terminator
};
const byte LOCKED_V2_IDENTITY[8] = {0x52, 0x44, 0x4E, 0x56, 0x02, 0x02, 0x4C, 0xA5};
const byte LOCKED_MICRO_IDENTITY[8] = {0x52, 0x44, 0x4E, 0x56, 0x02, 0x03, 0x4C, 0xA5};
const byte FLEXIBLE_MICRO_IDENTITY[8] = {0x52, 0x44, 0x4E, 0x56, 0x02, 0x03, 0x46, 0xA5};
const byte LEGACY_V2_IDENTITY[8] = {0x52, 0x44, 0x4E, 0x56, 0x01, 0x02, 0xFD, 0xA5};
const byte LEGACY_MICRO_IDENTITY[8] = {0x52, 0x44, 0x4E, 0x56, 0x01, 0x03, 0xFC, 0xA5};

bool identityMatches() {
  for (byte index = 0; index < sizeof(V2_IDENTITY); index++) {
    if (EEPROM.read(IDENTITY_ADDRESS + index) != V2_IDENTITY[index]) {
      return false;
    }
  }
  return true;
}

bool storedIdentityMatches(const byte expected[8]) {
  for (byte index = 0; index < 8; index++) {
    if (EEPROM.read(IDENTITY_ADDRESS + index) != expected[index]) {
      return false;
    }
  }
  return true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);

  if (identityMatches() || storedIdentityMatches(LOCKED_V2_IDENTITY) || storedIdentityMatches(LEGACY_V2_IDENTITY)) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Rednova V2 identity is already assigned. No change was made.");
    return;
  }

  if (storedIdentityMatches(LOCKED_MICRO_IDENTITY) || storedIdentityMatches(LEGACY_MICRO_IDENTITY)) {
    Serial.println("BLOCKED: ICSP-provisioned Rednova Micro identity cannot be changed to V2.");
    return;
  }

  bool blank = true;
  for (byte index = 0; index < 8; index++) {
    if (EEPROM.read(IDENTITY_ADDRESS + index) != 0xFF) blank = false;
  }
  if (!blank && !storedIdentityMatches(FLEXIBLE_MICRO_IDENTITY)) {
    Serial.println("BLOCKED: Unknown or protected identity data was found.");
    return;
  }

  for (byte index = 0; index < sizeof(V2_IDENTITY); index++) {
    EEPROM.update(IDENTITY_ADDRESS + index, V2_IDENTITY[index]);
  }

  if (identityMatches()) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Flexible Leonardo identity changed to Rednova V2 successfully.");
    Serial.println("Operation completed. Select the Rednova V2 board for future uploads.");
  } else {
    Serial.println("ERROR: Rednova V2 factory identity verification failed.");
  }
}

void loop() {}
