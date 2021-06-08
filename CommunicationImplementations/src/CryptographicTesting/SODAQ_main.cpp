#include <Sodaq_nbIOT.h>
#include <Sodaq_wdt.h>
#include <Encryption_testing.h>

#define R4XX
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial1
#define powerPin SARA_ENABLE
#define enablePin -1
#define DEBUG_STREAM_BAUD 115200
#define STARTUP_DELAY 5000
#define AES_GCM_ENCRYPTION 1
#define ACORN_ENCRYPTION 2
#define ASCON_ENCRYPTION 3

uint8_t cid = 1;
Sodaq_nbIOT nbiot;

void setup(){
    sodaq_wdt_safe_delay(STARTUP_DELAY);

    DEBUG_STREAM.begin(DEBUG_STREAM_BAUD);
    MODEM_STREAM.begin(nbiot.getSaraR4Baudrate());
    nbiot.setDiag(DEBUG_STREAM);
    nbiot.init(MODEM_STREAM, powerPin, enablePin, SARA_R4XX_TOGGLE, cid);

    /** Define initial text **/
    uint8_t inputUint8[MESSAGECONTAINERSIZE];

    for (unsigned char & index : inputUint8) {
        index = 0x01;
    }

    /** Allow the plaintext to vary in size **/
    const int textSize = sizeof(MESSAGECONTAINERSIZE);

    /** Convert char array to plaintext format; uint8_t **/
    uint8_t plaintext[textSize];

    /** Validation process **/
    uint8_t Tag[SIZE_16];
    uint8_t IV[SIZE_16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};
    uint8_t ciphertextReceiver[textSize] = {};

    /** Perform encryption and timings **/
    cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, textSize, cipher.key, IV, true);

    /** Plaintext buffer **/
    uint8_t plaintextReceiver[MESSAGECONTAINERSIZE];

    /** Perform decryption and timing of the following **/
    cipher.decryption.ascon_decryption(ciphertextReceiver, plaintextReceiver, Tag, textSize, cipher.key, IV, true);

}

void loop(){

}