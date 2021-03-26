#include "Sodaq_nbIOT.h"
#include "Sodaq_wdt.h"

#define R4XX
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial1
#define powerPin SARA_ENABLE
#define enablePin -1
#define DEBUG_STREAM_BAUD 115200
#define STARTUP_DELAY 5000

const char* apn = "company.iot.dk1.tdc";
const char* cdp = "10.212.187.119";
uint8_t cid = 1;
//const uint8_t band = 20;
//const char* forceOperator = "23801"; // optional - depends on SIM / network

Sodaq_nbIOT nbiot;

void sendMessageThroughUDP()
{
    DEBUG_STREAM.println();
    DEBUG_STREAM.println("Sending message through UDP");

//    int localPort = 16666;
    int socketID = nbiot.createSocket();

    if (socketID >= 7 || socketID < 0) {
        DEBUG_STREAM.println("Failed to create socket");
        return;
    }

    DEBUG_STREAM.println("Created socket!");

    int lengthSent = nbiot.socketSend(socketID,"40.85.115.125", 9889, "nybesked");
    DEBUG_STREAM.println(lengthSent);


    nbiot.closeSocket(socketID);

}

void setup()
{
    sodaq_wdt_safe_delay(STARTUP_DELAY);

    DEBUG_STREAM.begin(DEBUG_STREAM_BAUD);
    DEBUG_STREAM.println("Initializing and connecting... ");

    MODEM_STREAM.begin(nbiot.getSaraR4Baudrate());
    nbiot.setDiag(DEBUG_STREAM);
    DEBUG_STREAM.println("Initializing and connecting... 2");
    nbiot.init(MODEM_STREAM, powerPin, enablePin, SARA_R4XX_TOGGLE, cid);
    DEBUG_STREAM.println("Initializing and connecting... 3");
    unsigned long start = millis();
    DEBUG_STREAM.println("Time started now");
    //isConnected


    if (nbiot.connect(apn, cdp)) {
        DEBUG_STREAM.println("Connected succesfully!");
    }
    unsigned long end = millis();
    unsigned long delta = end - start;
    DEBUG_STREAM.println(delta);
    DEBUG_STREAM.println("Initializing and connecting... 4");

    sendMessageThroughUDP();
}

void loop()
{
    sodaq_wdt_safe_delay(60000);
    if (!nbiot.isConnected()) {
        if (!nbiot.connect(apn, cdp)) {
            DEBUG_STREAM.println("Failed to connect to the modem!");
        }
    }
    else {
        sendMessageThroughUDP();
    }
}