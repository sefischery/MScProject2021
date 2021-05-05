#ifndef MSCPROJECT2021_ASCON_H
#define MSCPROJECT2021_ASCON_H

bool ascon_encryption(uint8_t *plaintext,
                      uint8_t *ciphertext,
                      uint8_t *Tag,
                      int size,
                      uint8_t *key,
                      uint8_t *IV,
                      bool timingRequired = false);

bool ascon_decryption(uint8_t *ciphertext,
                      uint8_t *plaintext,
                      uint8_t *Tag,
                      int size,
                      uint8_t *key,
                      uint8_t *IV,
                      bool timingRequired = false);

#endif //MSCPROJECT2021_ASCON_H
