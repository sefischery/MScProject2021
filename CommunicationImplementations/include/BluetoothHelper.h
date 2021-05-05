#ifndef COMMUNICATIONIMPLEMENTATIONS_BLUETOOTHHELPER_H
#define COMMUNICATIONIMPLEMENTATIONS_BLUETOOTHHELPER_H

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *tag,
                       uint8_t *iv);

void performDecryption(uint8_t *ciphertext, uint8_t *tag, uint8_t *iv,
                       int ciphertextSize);

#endif //COMMUNICATIONIMPLEMENTATIONS_BLUETOOTHHELPER_H
