#include <Arduino.h>
#include <Encryption_testing.h>
#include "utilities.h"

void setup() {
    Serial.begin(115200);
    Serial.println();

    /** Define initial text **/
    uint8_t inputUint8[MESSAGECONTAINERSIZE];
    GenerateInitializationVector(inputUint8, MESSAGECONTAINERSIZE);

    /** Allow the plaintext to vary in size **/
    const int textSize = sizeof(MESSAGECONTAINERSIZE);

    /** Convert char array to plaintext format; uint8_t **/
    uint8_t plaintext[textSize];

    /** Validation process **/
    uint8_t tag[SIZE_16];
    uint8_t iv[SIZE_16];
    uint8_t ciphertextReceiver[textSize] = {};

    /** IV initialization **/
    GenerateInitializationVector(iv, SIZE_16);

    /** Perform encryption and timings **/
    cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, true);

    /** Plaintext buffer **/
    uint8_t plaintextReceiver[MESSAGECONTAINERSIZE];

    /** Perform decryption and timing of the following **/
    cipher.decryption.aes_gcm_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, true);
}

void loop() {

}