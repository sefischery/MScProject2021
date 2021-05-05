//
// Created by glasd on 4/29/2021.
//

#ifndef COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
#define COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H

/** Defining content **/
const char message[] = "Hej med dig";
const int messageSize = sizeof(message);

/** Preparation for encryption **/
uint8_t tag[16] = {};
uint8_t iv[16] = {};
uint8_t aesCiphertext[messageSize] = {};
uint8_t acornCiphertext[messageSize] = {};
uint8_t asconCiphertext[messageSize] = {};

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV);
void performDecryption(uint8_t *ciphertext, uint8_t *Tag, uint8_t *IV,
                       int ciphertextSize);

#endif //COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
