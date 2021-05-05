#ifndef COMMUNICATIONIMPLEMENTATIONS_BLE_CRYPTO_H
#define COMMUNICATIONIMPLEMENTATIONS_BLE_CRYPTO_H

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
