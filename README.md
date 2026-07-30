# Rednova Family

Arduino board support and library for the Robotzade Rednova controller family:

- Rednova V2
- Rednova Mega
- Rednova Micro

All three boards currently use the ATmega32U4 at 16 MHz and the Leonardo pin
mapping. Card-specific motor driver behavior is isolated in the library and
will be finalized after hardware testing.

## Arduino IDE installation

Add this URL to **File > Preferences > Additional Boards Manager URLs**:

```text
https://raw.githubusercontent.com/robotzade/Rednova-Family/main/package_rednova_index.json
```

Then open **Boards Manager**, search for **Rednova AVR Boards**, and install the
package. The Rednova library and examples are bundled with the board package.
Custom manufacturing bootloaders are intentionally excluded from the public
repository and Boards Manager archive.

## Board identification

The development build uses pid.codes test identifiers:

| Board | Bootloader | Sketch |
| --- | --- | --- |
| Rednova V2 | `1209:0001` | `1209:0002` |
| Rednova Mega | `1209:0003` | `1209:0004` |
| Rednova Micro | `1209:0005` | `1209:0006` |

These identifiers are only for development and testing. Production hardware
must use USB identifiers assigned to Robotzade/Rednova.

## Repository layout

- `hardware/rednova/avr`: local-development board platform
- `examples`: examples grouped by board
- `dist/package_rednova_index.json`: Boards Manager index source
- `Rednova.*`: current shared library implementation
