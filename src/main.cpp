#include <Arduino.h>
#include <utilities.h>
#include <acorn.h>

#define SIZE 16

void setup() {
    Serial.begin(115200);

    /** Define initial text **/
    char input[] = "hej med dig min ven";

    /** Allow the plaintext to vary in size **/
    const int textSize = sizeof(input);

    /** Setup for readable text to plaintext **/
    char backToText[textSize];

    /** Convert char array to plaintext format; uint8_t **/
    uint8_t plaintext[textSize];
    charToUint8(input, plaintext, textSize);

    /** Validation process **/
    uint8_t tag                 [SIZE];
    uint8_t plaintextReceiver   [textSize];
    uint8_t ciphertextReceiver  [textSize];

    /** Key and IV initialization **/
    uint8_t key[SIZE]                        = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    uint8_t iv[SIZE]                         = {0x00, 0x03, 0x06, 0x09, 0x0c, 0x0f, 0x12, 0x15, 0x18, 0x1b, 0x1e, 0x21, 0x24, 0x27, 0x2a, 0x2d};

    /** Encryption **/
    acorn_encryption(plaintext, ciphertextReceiver, tag, textSize, key, iv, SIZE);
    print_uint8(ciphertextReceiver, textSize);

    /** Decryption **/
    acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, key, iv, SIZE);
    print_uint8(plaintextReceiver, textSize);

    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, backToText, textSize);
    print_char(backToText, textSize);
}

void loop() {
}