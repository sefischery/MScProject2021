#ifndef COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
#define COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H


//WiFi information for the setup.
#define AP_SSID "Testfi"
#define UDP_PORT 1500
#define AES_GCM_ENCRYPTION 1
#define ACORN_ENCRYPTION 2
#define ASCON_ENCRYPTION 3

#define ENABLE_ENCRYPTION true



void performEncryption(int encryptionType,
                       uint8_t *plaintext,
                       int inputSize,
                       uint8_t *ciphertextReceiver,
                       uint8_t *Tag,
                       uint8_t *IV);

void performDecryption(uint8_t *ciphertext,
                       uint8_t *Tag,
                       uint8_t *IV,
                       int ciphertextSize);

void Build_And_Send_UDP_Packet(WiFiUDP &WiFiUDP,
                               char const *msg,
                               IPAddress &address,
                               int UDP_Port_Number);

void Build_And_Send_UDP_Packet(WiFiUDP &WiFiUDP,
                               const uint8_t *msg,
                               int msg_size,
                               IPAddress &address,
                               int UDP_Port_Number);

String Receive_UDP_Packet(WiFiUDP &WiFiUDP,
                          char packet_buffer[]);

void Connect_To_AP(const char *SSID,
                   WiFiClass &WiFi_instance);

#endif //COMMUNICATIONIMPLEMENTATIONS_WIFI_HELPER_H
