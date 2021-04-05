#include <Sodaq_nbIOT.h>
#include <NB-IoT_functions.h>


void sendNBIoTUDP(const char *message, const char *remoteIp, int remotePort, Sodaq_nbIOT &nbiot)
{
    DEBUG_STREAM.println("\n ---- Sending message through UDP ----");

    int socketID = nbiot.createSocket();

    if (socketID >= 7 || socketID < 0) {
        DEBUG_STREAM.println("Failed to create socket");
        return;
    }

    DEBUG_STREAM.println("Created socket!");

    int lengthSent = nbiot.socketSend(socketID, remoteIp, remotePort, message);
    DEBUG_STREAM.println(lengthSent);
    nbiot.closeSocket(socketID);

}