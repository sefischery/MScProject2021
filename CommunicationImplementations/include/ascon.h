//
// Created by glasd on 25-02-2021.
//

#ifndef MSCPROJECT2021_ASCON_H
#define MSCPROJECT2021_ASCON_H

bool ascon_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize);
bool ascon_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize);

#endif //MSCPROJECT2021_ASCON_H
