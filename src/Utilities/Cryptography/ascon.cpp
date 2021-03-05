#include <Arduino.h>
#include <Ascon128.h>

bool ascon_encryption(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {

    /** Initiate the Acorn cipher **/
    Ascon128 gcmAesEncryption;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the encryption process **/
    gcmAesEncryption.clear();
    gcmAesEncryption.setKey(key, cipherSize);
    gcmAesEncryption.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(tag, 0xBA, cipherSize);
    memset(ciphertext, 0xBA, size);

    /** Perform the encryption og compute the tag **/
    gcmAesEncryption.encrypt(ciphertext, plaintext, size);
    gcmAesEncryption.computeTag(tag, size);
    return true;
}

bool ascon_decryption(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag,
                        int size, uint8_t *key, uint8_t *iv, int cipherSize) {

    /** Initiate the Acorn cipher **/
    Ascon128 cipherDecryption;

    /** It is important to clear the key and iv before usage, ensures no strange rotations are affecting the decryption process **/
    cipherDecryption.clear();
    cipherDecryption.setKey(key, cipherSize);
    cipherDecryption.setIV(iv, cipherSize);

    /** Clear memory address values before using it. This ensures no weird values suddently appearing **/
    memset(plaintext, 0xBA, size);

    /** Perform the decryption **/
    cipherDecryption.decrypt(plaintext, ciphertext, size);

    /** Validate the received tag **/
    if (!cipherDecryption.checkTag(tag, cipherSize)) {
        return false;
    }

    return true;
}