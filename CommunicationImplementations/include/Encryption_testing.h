#include <Arduino.h>
#include <aes_gcm.h>
#include <ascon.h>
#include <utilities.h>
#include <acorn.h>
#include <Crypto.h>

#ifndef COMMUNICATIONIMPLEMENTATIONS_ENCRYPTION_TESTING_H
#define COMMUNICATIONIMPLEMENTATIONS_ENCRYPTION_TESTING_H

#define SIZE 16
#define MESSAGECONTAINERSIZE 128

struct encryption {
    bool (*acorn_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*aes_gcm_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*ascon_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
};

struct decryption {
    bool (*acorn_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*aes_gcm_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*ascon_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
};

struct cipherOperator {
    struct encryption encryption;
    struct decryption decryption;
    uint8_t key[SIZE];
};

struct cipherOperator cipher = {
        {
                /** Defined encryption methods **/
                acorn_encryption,
                aes_gcm_encryption,
                ascon_encryption
        },{
                /** Defined decryption methods **/
                acorn_decryption,
                aes_gcm_decryption,
                ascon_decryption
        },{
                /** Key definition **/
                0x00, 0x01, 0x02, 0x03, 0x04,
                0x05, 0x06, 0x07, 0x08, 0x09,
                0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
                0x0f
        }
};

#endif //COMMUNICATIONIMPLEMENTATIONS_ENCRYPTION_TESTING_H
