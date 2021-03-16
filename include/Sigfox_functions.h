#ifndef MSCPROJECT2021_SIGFOX_FUNCTIONS_H
#define MSCPROJECT2021_SIGFOX_FUNCTIONS_H
#define SER_RX_PIN D5
#define SER_TX_PIN D6
#define AT_COMM_TEST "AT\r"
#define AT_MODID_COMMAND "AT$I=10\r"
#define AT_PAC_COMMAND "AT$I=11\r"

void sendMessage(uint8_t *msg, int size, SoftwareSerial &softwareSerial);
void sendSigfoxATCommand(char const *Command, SoftwareSerial &softwareSerial);

#endif //MSCPROJECT2021_SIGFOX_FUNCTIONS_H
