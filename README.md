# Rednova Family

Rednova Family is an independent Robotzade controller platform compatible with
the Arduino IDE.

## Supported boards

- Rednova V2
- Rednova Micro

Rednova V2 uses an ATmega32U4 at 16 MHz and is compatible with the Arduino AVR
core and Leonardo pin mapping.

Rednova Micro uses the same processor, Leonardo pin mapping, motors, buzzer,
RGB LED, button, and trimpot as V2. Its DIP switch input has two positions:
SW1 on D15 and SW2 on D16. Rednova Mega will be added after its
model-specific hardware behavior is validated.

The bundled examples are organized into separate `RednovaV2` and
`RednovaMicro` groups. The Windows uploader validates the selected Rednova
model's application and bootloader USB identities before writing a sketch.
Each example is also compile-locked to its matching Rednova board selection,
so a Micro example cannot be built for V2 or a V2 example for Micro.

Boards manufactured with the original Arduino Leonardo bootloader use the
model-specific **Rednova V2 (Old Boot Leonardo)** or **Rednova Micro (Old Boot
Leonardo)** selection. Rednova stores a permanent model identity in the final
eight EEPROM bytes and verifies it before every upload. A V2 identity therefore
cannot be programmed through a Micro selection, even while the running sketch
uses the shared Leonardo USB identity.

Old boards without an EEPROM identity must be provisioned once with Arduino as
ISP: select the physical model's **Old Boot Leonardo** entry and run **Burn
Bootloader**. This writes the original Caterina bootloader and the matching
model identity. Normal sketch uploads preserve this identity.

## Development status

Rednova V2 has passed:

- Arduino IDE board discovery
- Compile and sketch upload
- USB serial communication
- Button, DIP switch, and trimpot tests
- Buzzer test
- WS2812B RGB LED test
- Motor speed and direction test
- Complete system test

## Installation

The board package is distributed through Arduino Boards Manager. During
development it can also be installed in the Arduino sketchbook `hardware`
directory. The standalone Rednova library ZIP remains available for projects
that use the API outside the bundled board examples.

## Product wording

> Rednova Family - compatible with the Arduino IDE

Rednova and Robotzade are independent brands. Arduino trademarks and logos are
not part of the Rednova product name or identity.

## Bootloader

The Rednova V2 Caterina bootloader, its Arduino source files, and the
reproducible patch tool are included in the public repository and release
archive. The same **Rednova V2** board selection can be used for normal sketch
uploads and for **Burn Bootloader** with **Arduino as ISP (Rednova Family)**.
