# Rednova Family Arduino IDE Compatibility

Rednova Family is an independent Robotzade product family designed to be
compatible with the Arduino IDE.

## Initial reference board

- Rednova V2

Rednova Mega and Rednova Micro will be added after the V2 bring-up tests pass.

## Common hardware foundation

- Microcontroller: ATmega32U4
- Clock: 16 MHz
- Arduino core compatibility: Arduino AVR core
- Pin mapping compatibility: Arduino Leonardo
- Sketch upload protocol: AVR109/Caterina
- Maximum sketch size: 28672 bytes
- Maximum SRAM usage: 2560 bytes

## Board compile identifiers

- Rednova V2: `ARDUINO_REDNOVA_V2` and `REDNOVA_BOARD_V2`
- Rednova Mega: `ARDUINO_REDNOVA_MEGA` and `REDNOVA_BOARD_MEGA`
- Rednova Micro: `ARDUINO_REDNOVA_MICRO` and `REDNOVA_BOARD_MICRO`

## USB development identifiers

| Board | Bootloader PID | Sketch PID |
| --- | --- | --- |
| Rednova V2 | `1209:0001` | `1209:0002` |
| Rednova Mega | `1209:0003` | `1209:0004` |
| Rednova Micro | `1209:0005` | `1209:0006` |

These identifiers are development identifiers. Production USB identifiers must
be obtained and registered for Robotzade before commercial distribution.

## Compatibility stages

1. [x] Arduino IDE lists Rednova V2.
2. [x] A basic sketch compiles for Rednova V2.
3. [x] USB serial communication works for Rednova V2.
4. [x] Rednova V2 has matching bootloader and sketch USB identities.
5. [x] Sketch upload works after the 1200-baud reset sequence.
6. The shared Rednova library selects model-specific behavior at compile time.
7. Examples compile for all supported models.
8. The signed release package installs through Boards Manager.

## V2 bring-up result

Tested successfully on 31 July 2026:

- Platform: Rednova Family 0.1.0
- Board: Rednova V2
- Sketch USB identity: `1209:0002`
- Basic LED test: passed
- USB serial test: passed
- Compile and upload: passed
- Button, DIP switch, and trimpot test: passed
- Buzzer test: passed
- WS2812B RGB LED test: passed
- Motor speed and direction test: passed
- Complete system test: passed
- Legacy Leonardo Caterina bootloaders remain upload-compatible, but do not
  provide a model-specific bootloader identity.

## Product wording

Recommended:

> Rednova Family - compatible with the Arduino IDE

The Arduino name and logo must not be used as part of the Rednova product name
or logo without written authorization from Arduino.
