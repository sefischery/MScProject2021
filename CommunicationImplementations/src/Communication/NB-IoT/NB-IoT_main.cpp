#include <Sodaq_nbIOT.h>
#include <Sodaq_wdt.h>
#include <NB-IoT_functions.h>
#include <utilities.h>

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
        DEBUG_STREAM.println("Initating NB-IoT connection");
        connected = nbiot.connect(AccessPointName, cdp);
        DEBUG_STREAM.println("Connected successfully!");
    }
    unsigned long connection_end = millis();
    unsigned long connection_delta = connection_end - connection_start;
    DEBUG_STREAM.print("It took ");
    DEBUG_STREAM.print(connection_delta);
    DEBUG_STREAM.println(" seconds to connect to the network");

    //sendNBIoTUDP("Succesfull Acknowledgement", TARGET_IP, TARGET_PORT, nbiot);
}

int msgCount = 0;
void loop()
{
    Serial.println("Entered LOOP");
    sodaq_wdt_safe_delay(30000);
    if (!nbiot.isConnected()) {
        Serial.println("Entered isConnected()");
        if (!nbiot.connect(AccessPointName, cdp)) {
            DEBUG_STREAM.println("Failed to connect to the modem!");
        }
    }
    else {
        // This is where the message is decrypted.
        /** Testing **/
        String msg = String("NB-IoT message; Number: ") + ++msgCount;

        uint8_t Tag[16];
        uint8_t IV[16];
        unsigned int msg_len = msg.length();
        uint8_t plaintext[msg_len];
        uint8_t ciphertext[msg_len];

        DEBUG_STREAM.println("Definition of arrays was successfull");

        // Copy message to plaintext as uint8_t
        charToUint8(msg.c_str(), plaintext, (int) msg_len);

        DEBUG_STREAM.println("Conversion of msg to plaintext was successful");

        // Perform encryption
        performEncryptionNB(AES_GCM_ENCRYPTION, plaintext, (int) msg_len, ciphertext, Tag, IV);

        DEBUG_STREAM.println("Encryption passed, and therefore were successful");

        // Concatenate to one big message containing IV + Tag + ciphertext
        uint8_t concatenatedMessage[sizeof(IV) + sizeof (Tag) + sizeof(ciphertext)];
        int concatenatedMessageSize = sizeof(IV) + sizeof (Tag) + sizeof(ciphertext);
        AssembleAuthenticatedEncryptionPacket(IV, Tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

        DEBUG_STREAM.println("Assemble function was passed without problems");

        /** Testing **/

        sendNBIoTUDP(concatenatedMessage, concatenatedMessageSize, TARGET_IP, TARGET_PORT, nbiot);

        // This could be included to
        //sendNBIoTUDP("This is clear-text, meaning not encrypted.", TARGET_IP, TARGET_PORT, nbiot);
    }
}