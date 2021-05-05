#include <Sodaq_nbIOT.h>
#include <NB-IoT_functions.h>
#include <Encryption_testing.h>

void sendNBIoTUDP(const uint8_t *message, int messageSize, const char *targetIP,
                  int targetPort, Sodaq_nbIOT &nbiot) {
    DEBUG_STREAM.println("\n ---- Sending message as UDP ----");

    int socketID = nbiot.createSocket();

    if (socketID >= 7 || socketID < 0) {
        DEBUG_STREAM.println("Failed to create socket");
        return;
    }

    DEBUG_STREAM.println("Created socket!");

    int lengthSent = nbiot.socketSend(socketID, targetIP, targetPort, message, messageSize);

    print_uint8(message, messageSize);

    DEBUG_STREAM.println(lengthSent);
    nbiot.closeSocket(socketID);
    DEBUG_STREAM.println("Socket closed");
}

void performEncryptionNB(int encryptionType, uint8_t *plaintext, int inputSize,
                         uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV) {
    /** IV initialization **/
    DEBUG_STREAM.println("Started IV generation");
    for (int index = 0; index < 16; ++index) {
        IV[index] = index;
    }
    DEBUG_STREAM.println("Generate IV");

    /** Perform encryption and timings **/
    if (encryptionType == AES_GCM_ENCRYPTION)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == ACORN_ENCRYPTION)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == ASCON_ENCRYPTION)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
}