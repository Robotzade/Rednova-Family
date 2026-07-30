# Rednova Boards Manager Distribution

Files prepared for release:

- `rednova-avr-1.0.0.zip`: self-contained Rednova AVR platform
- `package_rednova_index.json`: Arduino Boards Manager index

The platform archive contains the AVR core, Leonardo variant, upload support,
the Rednova library, and all examples for Rednova V2, Mega, and Micro.

Before publishing:

1. Upload `rednova-avr-1.0.0.zip` to a permanent HTTPS address.
2. Replace `REPLACE_WITH_PUBLIC_URL` in `package_rednova_index.json`.
3. Replace `REPLACE_WITH_SUPPORT_EMAIL` with the public support email.
4. Upload `package_rednova_index.json` to a permanent HTTPS address.
5. Add that `_index.json` URL under Arduino IDE:
   **File > Preferences > Additional Boards Manager URLs**.
6. Open Boards Manager, search for **Rednova AVR Boards**, and install it.

Do not modify the ZIP after publishing. If it changes, recalculate both its
SHA-256 checksum and byte size in the index.
