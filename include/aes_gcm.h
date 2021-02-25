//
// Created by glasd on 25-02-2021.
//

#ifndef MSCPROJECT2021_AES_GCM_H
#define MSCPROJECT2021_AES_GCM_H

void aes_gcm_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize);
void aes_gcm_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize);

#endif //MSCPROJECT2021_AES_GCM_H
