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

Boards manufactured with the original Arduino Leonardo bootloader can use the
matching `Legacy Leonardo Bootloader` V2 or Micro board selection. These
entries retain the Leonardo USB identity while defining the correct Rednova
model for the library and examples.

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

The release package is distributed through Arduino Boards Manager. During
development it can also be installed in the Arduino sketchbook `hardware`
directory.

## Product wording

> Rednova Family - compatible with the Arduino IDE

Rednova and Robotzade are independent brands. Arduino trademarks and logos are
not part of the Rednova product name or identity.

## Bootloader

The Rednova V2 Caterina bootloader, its Arduino source files, and the
reproducible patch tool are included in the public repository and release
archive. The same **Rednova V2** board selection can be used for normal sketch
uploads and for **Burn Bootloader** with **Arduino as ISP (Rednova Family)**.
