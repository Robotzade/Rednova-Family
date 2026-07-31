#include <EEPROM.h>
#include <avr/pgmspace.h>

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
enum BootloaderModel { BOOT_UNKNOWN, BOOT_LEONARDO, BOOT_V2, BOOT_MICRO };

BootloaderModel detectBootloaderModel() {
  const byte marker0 = pgm_read_byte_far(0x7F4F);
  const byte marker1 = pgm_read_byte_far(0x7F50);
  const byte marker2 = pgm_read_byte_far(0x7F51);

  if (marker0 == 0x41 && marker1 == 0x23 && marker2 == 0x36) return BOOT_LEONARDO;
  if (marker0 == 0x09 && marker1 == 0x12 && marker2 == 0x01) return BOOT_V2;
  if (marker0 == 0x09 && marker1 == 0x12 && marker2 == 0x03) return BOOT_MICRO;
  return BOOT_UNKNOWN;
}

bool identityMatches() {
  for (byte index = 0; index < sizeof(V2_IDENTITY); index++) {
    if (EEPROM.read(IDENTITY_ADDRESS + index) != V2_IDENTITY[index]) {
      return false;
    }
  }
  return true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(9600);

  const BootloaderModel bootloader = detectBootloaderModel();
  if (bootloader == BOOT_V2) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Rednova V2 ICSP bootloader detected. Model is permanently locked to V2.");
    return;
  }
  if (bootloader == BOOT_MICRO) {
    Serial.println("BLOCKED: Rednova Micro ICSP bootloader cannot be changed to V2.");
    return;
  }
  if (bootloader != BOOT_LEONARDO) {
    Serial.println("BLOCKED: An authorized Leonardo bootloader was not detected.");
    return;
  }

  if (identityMatches()) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Rednova V2 identity is already assigned. No change was made.");
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
