//
// Created by glasd on 23-02-2021.
//

#ifndef MSCPROJECT2021_ACORN_H
#define MSCPROJECT2021_ACORN_H

void acorn_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                      int size, uint8_t *key, uint8_t *iv, int cipherSize);

void acorn_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                      int size, uint8_t *key, uint8_t *iv, int cipherSize);

#endif //MSCPROJECT2021_ACORN_H
