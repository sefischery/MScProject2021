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
        DEBUG_STREAM.println("Initiating NB-IoT connection");
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
        DEBUG_STREAM.println("Successful connection to NB-IoT modem");

        /** Define message **/
        String msg = String("NB-IoT message; Number: ") + ++msgCount;

        /** Encryption process started **/
        uint8_t Tag[16];
        uint8_t IV[16];
        unsigned int msg_len = msg.length();
        uint8_t plaintext[msg_len];
        uint8_t ciphertext[msg_len];

        /** Print plaintext **/
        DEBUG_STREAM.print("Plain data: \"");
        DEBUG_STREAM.print(msg);
        DEBUG_STREAM.println("\"");

        /** Copy message to plaintext as uint8_t **/
        charToUint8(msg.c_str(), plaintext, (int) msg_len);

        /** Perform encryption **/
        performEncryptionNB(AES_GCM_ENCRYPTION, plaintext, (int) msg_len, ciphertext, Tag, IV);

        /** Concatenate to one big message containing IV + Tag + ciphertext **/
        uint8_t concatenatedMessage[sizeof(IV) + sizeof (Tag) + sizeof(ciphertext)];
        int concatenatedMessageSize = sizeof(IV) + sizeof (Tag) + sizeof(ciphertext);
        AssembleAuthenticatedEncryptionPacket(IV, Tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

        /** Print encrypted contetn **/
        DEBUG_STREAM.println("");
        DEBUG_STREAM.print("Encrypted data: ");
        for (int index = 0; index < concatenatedMessageSize; ++index) {
            int num = concatenatedMessage[index];
            char hex[6];
            sprintf(hex, "0x%02x ", num);
            DEBUG_STREAM.print(hex);
        }DEBUG_STREAM.println("");
        DEBUG_STREAM.println("");

        /** Send assembled encrypted packet **/
        sendNBIoTUDP(concatenatedMessage, concatenatedMessageSize, TARGET_IP, TARGET_PORT, nbiot);

        /** Unecrypted version of NB-IoT
        DEBUG_STREAM.println("Sending *UNENCRYPTED* NB-IoT packet: ");
        const uint8_t message[] = "This is an unencrypted NB-IoT packet";
        sendNBIoTUDP(message, (int) sizeof(message), TARGET_IP, TARGET_PORT, nbiot);
        **/
    }
}