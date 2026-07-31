#include "Arduino.h"
#include "Rednova.h"

#define M2_Pwm 10
#define M2_Dir 5
#define M1_Pwm 9
#define M1_Dir 11
#define BZR 13
#define LED_BIT 6 // D7 = PE6
#define LED_PORT PORTE // Port E
#define LED_DDR DDRE // DDR E
#define BUTTON_PIN 14
#define TRIMPOT_PIN A11 // Trimpot bağlı pin

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

int melody[] = { NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4 };
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4 };

String RednovaReadString;

RednovaClass::RednovaClass()
    : TrimpotState(0), ButtonState(0),
#if defined(REDNOVA_BOARD_MICRO)
      SwitchState("00") {
#else
      SwitchState("0000") {
#endif
}

void RednovaClass::SETUP(){
    begin();
}

void RednovaClass::begin() {
    LED_DDR |= (1 << LED_BIT);
    LED_PORT &= ~(1 << LED_BIT);
    pinMode(BZR ,OUTPUT);
    pinMode(M1_Pwm, OUTPUT);
    pinMode(M1_Dir, OUTPUT);
    pinMode(M2_Pwm, OUTPUT);
    pinMode(M2_Dir, OUTPUT);
    analogWrite(M1_Pwm, 0);
    analogWrite(M2_Pwm, 0);
    pinMode(15, INPUT_PULLUP);
    pinMode(16, INPUT_PULLUP);
#if !defined(REDNOVA_BOARD_MICRO)
    pinMode(17, INPUT_PULLUP);
    pinMode(30, INPUT_PULLUP);
#endif
    pinMode(14, INPUT_PULLUP);
    pinMode(TRIMPOT_PIN, INPUT);
}

void RednovaClass::Buzzer(boolean x , int Time ){
    if (x == 1) {
        tone(13, NOTE_A3);
        delay(Time);
    } else if (x == 0) {
        noTone(13);
        delay(Time);
    }
}

void RednovaClass::ReadSwitch() {
#if defined(REDNOVA_BOARD_MICRO)
    char s[3]; // 2 switches + null terminator
    s[0] = (digitalRead(15) == LOW) ? '1' : '0'; // SW1
    s[1] = (digitalRead(16) == LOW) ? '1' : '0'; // SW2
    s[2] = '\0';
#else
    char s[5]; // 4 karakter + null terminator
    s[0] = (digitalRead(15) == LOW) ? '1' : '0'; // Bit0
    s[1] = (digitalRead(16) == LOW) ? '1' : '0'; // Bit1
    s[2] = (digitalRead(17) == LOW) ? '1' : '0'; // Bit2
    s[3] = (digitalRead(30) == LOW) ? '1' : '0'; // Bit3
    s[4] = '\0';
#endif
    SwitchState = String(s);
}

void RednovaClass::ReadButton() {
    if (digitalRead(BUTTON_PIN) == LOW) ButtonState = 1; // Basılı
    else ButtonState = 0; // Basılı değil
}

void RednovaClass::ReadTrimpot() {
    TrimpotState = analogRead(TRIMPOT_PIN);
}

void RednovaClass::StartMelody(){
    delay(300);
    for (int thisNote = 0; thisNote < 8; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(13, melody[thisNote], noteDuration);
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(13);
    }
}

void RednovaClass::DualDirection(float Lval, float Rval, int Time){
    WriteMotor(M1_Pwm, M1_Dir, Lval);
    WriteMotor(M2_Pwm, M2_Dir, Rval);
    if (Time > 0) {
        delay(Time);
    }
}

void RednovaClass::WriteMotor(uint8_t pwmPin, uint8_t directionPin, float value) {
    value = constrain(value, -100.0f, 100.0f);
    bool forward = value >= 0.0f;
    int pwm = (int)(abs(value) * 2.55f + 0.5f);

    // All boards currently use the Rednova V2 motor behavior.
    // Card-specific motor driver differences will be added here after testing.
#if defined(REDNOVA_BOARD_MEGA)
    digitalWrite(directionPin, forward ? HIGH : LOW);
#elif defined(REDNOVA_BOARD_MICRO)
    digitalWrite(directionPin, forward ? HIGH : LOW);
#else
    digitalWrite(directionPin, forward ? HIGH : LOW);
#endif
    analogWrite(pwmPin, pwm);
}

RednovaBoardType RednovaClass::BoardType() const {
#if defined(REDNOVA_BOARD_MEGA)
    return REDNOVA_MEGA;
#elif defined(REDNOVA_BOARD_MICRO)
    return REDNOVA_MICRO;
#else
    return REDNOVA_V2;
#endif
}

const __FlashStringHelper* RednovaClass::BoardName() const {
#if defined(REDNOVA_BOARD_MEGA)
    return F("Rednova Mega");
#elif defined(REDNOVA_BOARD_MICRO)
    return F("Rednova Micro");
#else
    return F("Rednova V2");
#endif
}

void sendBit(bool bitVal) {
    if (bitVal) {
        LED_PORT |= (1 << LED_BIT);
        asm volatile( "nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                      "nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                      "nop\n\t""nop\n\t""nop\n\t""nop\n\t" );
        LED_PORT &= ~(1 << LED_BIT);
        asm volatile( "nop\n\t" );
    } else {
        LED_PORT |= (1 << LED_BIT);
        asm volatile( "nop\n\t" );
        LED_PORT &= ~(1 << LED_BIT);
        asm volatile( "nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                      "nop\n\t""nop\n\t""nop\n\t""nop\n\t"
                      "nop\n\t""nop\n\t""nop\n\t""nop\n\t" );
    }
}

void sendByte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {
        sendBit(byte & (1 << i));
    }
}

void RednovaClass::ColorFunction(uint8_t r, uint8_t g, uint8_t b) {
    // --- Maksimum parlaklık limiti (100) ---
    if (r > 100) r = 100;
    if (g > 100) g = 100;
    if (b > 100) b = 100;
    // Send the requested color.
    noInterrupts();
    sendByte(g); // WS2812 sırası: G, R, B
    sendByte(r);
    sendByte(b);
    interrupts();
    // WS2812 reset sinyali
    delayMicroseconds(50);
}

void RednovaClass::PlayStartup() {
    // --- Small delay before starting ---
    delay(300);

    // --- LED rocket/fade-up effect ---
    for (int brightness = 0; brightness <= 150; brightness += 5) {
        // RGB fade with different ratios for full color effect
        int r = brightness;       // Red channel
        int g = brightness / 2;   // Green channel
        int b = brightness / 3;   // Blue channel

        noInterrupts();
        sendByte(g);  // G
        sendByte(r);  // R
        sendByte(b);  // B
        interrupts();
        delayMicroseconds(50);

        // Buzzer rise effect on pin 13
        int freq = 200 + brightness * 8; // Start low, go higher
        tone(13, freq, 20);              // Short tone
        delay(20);                        // Delay for smooth ramp
    }

    // --- Small ending flourish ---
    for (int i = 150; i >= 0; i -= 5) {
        noInterrupts();
        sendByte(i / 2);  // G
        sendByte(i);      // R
        sendByte(i / 3);  // B
        interrupts();
        delayMicroseconds(50);
        delay(10);
    }

    // --- LED off ---
    noInterrupts();
    sendByte(0);
    sendByte(0);
    sendByte(0);
    interrupts();
    delayMicroseconds(50);
    delay(100);
}
void RednovaClass::Jump() {
    // --- Define pitches ---
    int jumpMelody[][2] = {
        {1047, 150}, // NOTE_C6 - Jump
        {1319, 100}, // NOTE_E6 - Hit
    };

    // --- Play each note ---
    const size_t jumpNoteCount = sizeof(jumpMelody) / sizeof(jumpMelody[0]);
    for (size_t i = 0; i < jumpNoteCount; i++) {
        tone(13, jumpMelody[i][0], jumpMelody[i][1]);
        delay(jumpMelody[i][1] * 1.3);
        noTone(13);
    }
}


void RednovaClass::PlayMario() {
    int marioMelody[] = {
        NOTE_E7, NOTE_E7, 0, NOTE_E7,
        0, NOTE_C7, NOTE_E7, 0,
        NOTE_G7, 0, 0, 0,
        NOTE_G6, 0, 0, 0,
        NOTE_C7, 0, NOTE_G6, 0,
        NOTE_E6, 0, NOTE_A6, NOTE_B6,
        0, NOTE_AS6, NOTE_A6, 0,
        NOTE_G6, NOTE_E7, NOTE_G7, NOTE_A7,
        NOTE_F7, NOTE_G7, 0, NOTE_E7,
        0, NOTE_C7, NOTE_D7, NOTE_B6
    };
    
    int marioDurations[] = {
        8, 8, 8, 8,
        8, 8, 8, 8,
        4, 8, 8, 8,
        4, 8, 8, 8,
        4, 8, 8, 8,
        4, 4, 4, 4,
        8, 8, 8, 8,
        4, 4, 4, 4,
        4, 8, 8, 4,
        4, 4, 4, 4
    };
    
    const size_t marioNoteCount = sizeof(marioMelody) / sizeof(marioMelody[0]);
    for (size_t thisNote = 0; thisNote < marioNoteCount; thisNote++) {
        int noteDuration = 1000 / marioDurations[thisNote];
        if (marioMelody[thisNote] != 0) {
            tone(BZR, marioMelody[thisNote], noteDuration);
        }
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        noTone(BZR);
    }
}

void RednovaClass::MixLed(){
    float scale = 1.25; // Parlaklık artırıcı katsayı
  
        for (int i = 0; i < 255; i++) {
            int rz = (sin(0.024 * i + 0) * 127 + 128) * scale;
            int gbz = (sin(0.024 * i + 2) * 127 + 128) * scale;
            int bz = (sin(0.024 * i + 4) * 127 + 128) * scale;
            // --- MAKSİMUM 190 SINIRI ---
            if (rz > 190) rz = 190;
            if (gbz > 190) gbz = 190;
            if (bz > 190) bz = 190;
            // ----------------------------
            noInterrupts(); sendByte(gbz); sendByte(rz); sendByte(bz); interrupts();
            delayMicroseconds(50);
            delay(3);
        }
   
}


RednovaClass Rednova;
