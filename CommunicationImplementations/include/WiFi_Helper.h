#ifndef COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
#define COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H

/** Defining content **/
const char message[] = "Hej med dig";
const int messageSize = sizeof(message);

//WiFi information for the setup.
#define AP_SSID "ESP32_AP"
#define UDP_PORT 1500

/** Preparation for encryption **/
uint8_t Tag[16] = {};
uint8_t IV[16] = {};
uint8_t aesCiphertext[messageSize] = {};
uint8_t acornCiphertext[messageSize] = {};
uint8_t asconCiphertext[messageSize] = {};

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV);
void performDecryption(uint8_t *ciphertext, uint8_t *Tag, uint8_t *IV,
                       int ciphertextSize);
void Build_And_Send_UDP_Packet(WiFiUDP &WiFiUDP, char const *msg, IPAddress &address, int UDP_Port_Number);

String Receive_UDP_Packet(WiFiUDP &WiFiUDP, char packet_buffer[]);

#endif //COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
