#ifndef MSCPROJECT2021_ACORN_H
#define MSCPROJECT2021_ACORN_H

bool acorn_encryption(uint8_t *plaintext,
                      uint8_t *ciphertext,
                      uint8_t *Tag,
                      int size,
                      uint8_t *key,
                      uint8_t *IV,
                      bool timingRequired);

bool acorn_decryption(uint8_t *ciphertext,
                      uint8_t *plaintext,
                      uint8_t *Tag,
                      int size,
                      uint8_t *key,
                      uint8_t *IV,
                      bool timingRequired);

#endif //MSCPROJECT2021_ACORN_H
