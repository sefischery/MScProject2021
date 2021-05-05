#include <Arduino.h>
#include <Encryption_testing.h>

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *Tag,
                       uint8_t *IV) {
    /** IV initialization **/
    GenerateInitializationVector(IV, 16);

    /** Perform encryption and timings **/
    if (encryptionType == 1)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 2)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 3)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
}

void performDecryption(uint8_t *ciphertext, uint8_t *Tag, uint8_t *IV,
                       int ciphertextSize) {
    /** Plaintext buffer **/
    uint8_t plaintextReceiver[ciphertextSize];
    char text[ciphertextSize];

    /** Perform decryption and timing of the following **/
    cipher.decryption.aes_gcm_decryption(ciphertext, plaintextReceiver, Tag, ciphertextSize, cipher.key, IV, false);

    Serial.print("Tag: ");
    print_uint8(Tag, 16);

    Serial.print("Iv: ");
    print_uint8(IV, 16);

    uint8ToChar(plaintextReceiver, text, ciphertextSize-32);
    Serial.print("Decrypted Text: ");
    print_char(text, ciphertextSize-32);
}
