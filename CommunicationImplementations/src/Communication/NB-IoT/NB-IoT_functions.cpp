#include <Sodaq_nbIOT.h>
#include <NB-IoT_functions.h>
#include <Encryption_testing.h>

/*
void sendNBIoTUDP(const char *message, const char *targetIP, int targetPort, Sodaq_nbIOT &nbiot)
{
    DEBUG_STREAM.println("\n ---- Sending message as UDP ----");

    int socketID = nbiot.createSocket();

    if (socketID >= 7 || socketID < 0) {
        DEBUG_STREAM.println("Failed to create socket");
        return;
    }

    DEBUG_STREAM.println("Created socket!");

    int lengthSent = nbiot.socketSend(socketID, targetIP, targetPort, message);
    DEBUG_STREAM.println(lengthSent);
    nbiot.closeSocket(socketID);

}*/

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
                       uint8_t *ciphertextReceiver, uint8_t *tag, uint8_t *iv) {
    /** IV initialization **/
    DEBUG_STREAM.println("Started iv generation");
    for (int index = 0; index < 16; ++index) {
        iv[index] = index;
    }
    DEBUG_STREAM.println("Generate IV");

    /** Perform encryption and timings **/
    if (encryptionType == 1)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, tag, inputSize, cipher.key, iv, SIZE, false);
    }
    else if (encryptionType == 2)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, tag, inputSize, cipher.key, iv, SIZE, false);
    }
    else if (encryptionType == 3)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, tag, inputSize, cipher.key, iv, SIZE, false);
    }
}

void performDecryption(uint8_t *ciphertext, uint8_t *tag, uint8_t *iv,
                       int ciphertextSize) {
    /** Plaintext buffer **/
    uint8_t plaintextReceiver[ciphertextSize];
    char text[ciphertextSize];

    /** Perform decryption and timing of the following **/
    cipher.decryption.aes_gcm_decryption(ciphertext, plaintextReceiver, tag, ciphertextSize, cipher.key, iv, SIZE, false);

    Serial.print("Tag: ");
    print_uint8(tag, 16);

    Serial.print("Iv: ");
    print_uint8(iv, 16);

    uint8ToChar(plaintextReceiver, text, ciphertextSize);
    Serial.print("Decrypted Text: ");
    print_char(text, ciphertextSize);
}