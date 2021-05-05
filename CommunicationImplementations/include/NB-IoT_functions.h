#include <Sodaq_nbIOT.h>
#ifndef COMMUNICATIONIMPLEMENTATIONS_NB_IOT_FUNCTIONS_H
#define COMMUNICATIONIMPLEMENTATIONS_NB_IOT_FUNCTIONS_H

//Definitions required by the library, specifically for SODAQ SARAR4XX
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

void sendNBIoTUDP(const uint8_t *message, int messageSize, const char *targetIP, int targetPort, Sodaq_nbIOT &nbiot);
void performEncryptionNB(int encryptionType, uint8_t *plaintext, int inputSize,
                         uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV);

#endif //COMMUNICATIONIMPLEMENTATIONS_NB_IOT_FUNCTIONS_H
