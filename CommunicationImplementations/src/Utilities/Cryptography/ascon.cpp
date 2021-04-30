#include <Arduino.h>
#include <Ascon128.h>

#define POINTITERATION 200

bool ascon_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {
    bool timingRequired = false;

    /** Initiate the Acorn cipher **/
    Ascon128 cipher;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the encryption process **/
    cipher.clear();
    cipher.setKey(key, cipherSize);
    cipher.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(tag, 0xBA, cipherSize);
    memset(ciphertext, 0xBA, size);

    /** Perform the encryption **/
    cipher.encrypt(ciphertext, plaintext, size);

    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("ascon-encryption,");
        /** Perform the encryption og compute the tag **/
        for (int graphIteration = 0; graphIteration < POINTITERATION; graphIteration++){
            start = micros();
            for (count = 0; count < 500; ++count) {
                cipher.encrypt(ciphertext, plaintext, size);
            }
            elapsed = micros() - start;

            Serial.print(elapsed / (size * 500.0));
            Serial.println(",");
            delay(10);
        }
    }

    /** Compute the tag **/
    cipher.computeTag(tag, size);
    return true;
}

bool ascon_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {
    bool timingRequired = false;

    /** Initiate the Acorn cipher **/
    Ascon128 cipher;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the decryption process **/
    cipher.clear();
    cipher.setKey(key, cipherSize);
    cipher.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(plaintext, 0xBA, size);

    /** Perform the decryption **/
    cipher.decrypt(plaintext, ciphertext, size);

    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("ascon-decryption,");
        /** Perform the encryption og compute the tag **/
        for (int graphIteration = 0; graphIteration < POINTITERATION; graphIteration++){
            start = micros();
            for (count = 0; count < 500; ++count) {
                cipher.decrypt(plaintext, ciphertext, size);
            }
            elapsed = micros() - start;

            Serial.print(elapsed / (size * 500.0));
            Serial.println(",");
        }
    }

    /** Validate the received tag **/
    if (!cipher.checkTag(tag, cipherSize)) {
        return false;
    }

    return true;
}