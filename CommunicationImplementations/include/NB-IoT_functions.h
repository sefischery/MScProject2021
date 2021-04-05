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

void sendNBIoTUDP(const char *message, const char *remoteIp, int remotePort, Sodaq_nbIOT &nbiot);

#endif //COMMUNICATIONIMPLEMENTATIONS_NB_IOT_FUNCTIONS_H
