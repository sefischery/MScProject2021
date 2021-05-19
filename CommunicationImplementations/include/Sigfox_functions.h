#ifndef MSCPROJECT2021_SIGFOX_FUNCTIONS_H
#define MSCPROJECT2021_SIGFOX_FUNCTIONS_H
#define SER_RX_PIN D5
#define SER_TX_PIN D6
#define AT_COMM_TEST "AT\r"
#define AT_MODID_COMMAND "AT$I=10\r"
#define AT_PAC_COMMAND "AT$I=11\r"

#define AES_GCM_ENCRYPTION 1
#define ACORN_ENCRYPTION 2
#define ASCON_ENCRYPTION 3

void sendSigfoxMessage(uint8_t *msg, int size, SoftwareSerial &softwareSerial);
void sendSigfoxATCommand(char const *Command, SoftwareSerial &softwareSerial);
void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize, uint8_t *ciphertextReceiver,
                       uint8_t *Tag, uint8_t *IV);

#endif //MSCPROJECT2021_SIGFOX_FUNCTIONS_H
