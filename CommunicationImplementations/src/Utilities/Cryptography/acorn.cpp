#include <Arduino.h>
#include <Acorn128.h>
#define SIZE_16 16
#define POINTITERATION 200


bool acorn_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv,
                      bool timingRequired = false) {
    /** Initiate the Acorn cipher **/
    Acorn128 cipher;

    /** It is important to clear the key and IV before usage, ensures no strange rotations are affecting the encryption process **/
    cipher.clear();
    cipher.setKey(key, SIZE_16);
    cipher.setIV(iv, SIZE_16);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(tag, 0xBA, SIZE_16);
    memset(ciphertext, 0xBA, size);

    cipher.encrypt(ciphertext, plaintext, size);

    /** Timings are started here **/
    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("acorn-encryption,");
        /** Perform the encryption og compute the Tag **/
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
    /** Timings are ended here **/

    cipher.computeTag(tag, size);

    return true;
}

bool acorn_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv,
                      bool timingRequired = false) {
    /** Initiate the Acorn cipher **/
    Acorn128 cipher;

    /** It is important to clear the key and IV before usage, ensures no strange rotations are affecting the decryption process **/
    cipher.clear();
    cipher.setKey(key, SIZE_16);
    cipher.setIV(iv, SIZE_16);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(plaintext, 0xBA, size);

    /** Perform the decryption **/
    cipher.decrypt(plaintext, ciphertext, size);

    /** Timings started here **/
    if (timingRequired) {
        unsigned long start;
        unsigned long elapsed;
        int count;
        Serial.println("acorn-decryption,");
        /** Perform the encryption og compute the Tag **/
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
    /** Timings ended here **/

    /** Validate the received Tag **/
    if (!cipher.checkTag(tag, SIZE_16)) {
        return false;
    }

    return true;
}
