#ifndef COMMUNICATIONIMPLEMENTATIONS_BLE_CRYPTO_H
#define COMMUNICATIONIMPLEMENTATIONS_BLE_CRYPTO_H
#define AES_GCM_ENCRYPTION 1
#define ACORN_ENCRYPTION 2
#define ASCON_ENCRYPTION 3

void performEncryption(int encryptionType,
                       uint8_t *plaintext,
                       int inputSize,
                       uint8_t *ciphertextReceiver,
                       uint8_t *Tag,
                       uint8_t *IV);

void performDecryption(uint8_t *ciphertext,
                       uint8_t *Tag,
                       uint8_t *IV,
                       int ciphertextSize);

#endif //COMMUNICATIONIMPLEMENTATIONS_BLE_CRYPTO_H
