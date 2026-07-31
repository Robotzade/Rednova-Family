# Rednova Family

Rednova Family is an independent Robotzade controller platform compatible with
the Arduino IDE.

## Supported board

- Rednova V2

Rednova V2 uses an ATmega32U4 at 16 MHz and is compatible with the Arduino AVR
core and Leonardo pin mapping.

Rednova Mega and Rednova Micro will be added after the V2 platform is released
and their model-specific hardware behavior is validated.

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

## Private manufacturing files

Model-specific factory bootloader files and manufacturing tools are excluded
from the public repository and release archives.
