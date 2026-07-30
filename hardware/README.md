# Rednova AVR Boards

This folder contains the development version of the Arduino board platform for:

- Rednova V2
- Rednova Mega
- Rednova Micro

All three definitions currently use the ATmega32U4 at 16 MHz, the Arduino
Leonardo core and variant, and the Rednova V2 motor behavior.

## Local test installation

1. Install **Arduino AVR Boards** from Arduino IDE's Boards Manager.
2. Copy the `rednova` folder from this directory to:
   `<Arduino sketchbook>/hardware/rednova`
3. Restart Arduino IDE.
4. Select the desired board under **Tools > Board > Rednova AVR Boards**.

The expected installed layout is:

```text
<Arduino sketchbook>/
  hardware/
    rednova/
      avr/
        boards.txt
        platform.txt
```

## Distribution note

Installing the Rednova library ZIP does not install this board platform.
For customer installation through Boards Manager, publish the `avr` platform
as a versioned archive and provide an Arduino `package_index.json` URL.

The current development definitions intentionally retain the Leonardo USB
VID/PID values so they work with the existing Leonardo-compatible bootloader.
Before public distribution, replace these values with Rednova's assigned USB
identifiers if available.
