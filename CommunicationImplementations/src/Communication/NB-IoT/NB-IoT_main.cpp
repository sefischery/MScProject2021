#include <Sodaq_nbIOT.h>
#include <Sodaq_wdt.h>
#include <NB-IoT_functions.h>

/** Definitions for the Azure VM, and the port it listens to.
 *  Used by the NB-IoT backend to route the UDP packet. **/
#define TARGET_IP "40.85.115.125"
#define TARGET_PORT 9889

/** Operator information, TDC endpoint and IP. **/
const char* AccessPointName = "company.iot.dk1.tdc";
const char* cdp = "10.212.187.119";
uint8_t cid = 1;
//const uint8_t band = 20;
//const char* forceOperator = "23801"; // optional - depends on SIM / network

Sodaq_nbIOT nbiot;

void setup()
{
    sodaq_wdt_safe_delay(STARTUP_DELAY);

    DEBUG_STREAM.begin(DEBUG_STREAM_BAUD);
    MODEM_STREAM.begin(nbiot.getSaraR4Baudrate());
    nbiot.setDiag(DEBUG_STREAM);
    nbiot.init(MODEM_STREAM, powerPin, enablePin, SARA_R4XX_TOGGLE, cid);

    unsigned long connection_start = millis();
    bool connected = false;
    while (!connected) {
        /** Sets connected flag if the .connect function returns true **/
        connected = nbiot.connect(AccessPointName, cdp);
        DEBUG_STREAM.println("Connected successfully!");
    }
    unsigned long connection_end = millis();
    unsigned long connection_delta = connection_end - connection_start;
    DEBUG_STREAM.print("It took ");
    DEBUG_STREAM.print(connection_delta);
    DEBUG_STREAM.println(" seconds to connect to the network");

    sendNBIoTUDP("First_Test_Message", TARGET_IP, TARGET_PORT, nbiot);
}

void loop()
{
    sodaq_wdt_safe_delay(60000);
    if (!nbiot.isConnected()) {
        if (!nbiot.connect(AccessPointName, cdp)) {
            DEBUG_STREAM.println("Failed to connect to the modem!");
        }
    }
    else {
        sendNBIoTUDP("NBIoT-Test-Message", TARGET_IP, TARGET_PORT, nbiot);
    }
}