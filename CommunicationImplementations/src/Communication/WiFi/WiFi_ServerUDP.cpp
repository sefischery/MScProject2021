#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>
#include <cstring>
#include <utilities.h>
#include <Base64.h>

WiFiUDP UDP;
//WiFi information for the setup.
#define AP_CHANNEL 1
#define AP_CLOACK 0
#define AP_MAX_CON 5
//#define AP_PASSWORD ""

/** Preparation for encryption **/
uint8_t Tag[16] = {};
uint8_t IV[16] = {};
char UDP_RECEIVER_BUFFER[350];

void Create_Open_AccessPoint(const char *SSID,
                             int WiFi_Channel,
                             int Hide_Access_Point,
                             int Max_Number_of_Connections) {

    WiFi.softAP(SSID,nullptr,WiFi_Channel,Hide_Access_Point,Max_Number_of_Connections);
    Serial.println("Assigned IP address of Access Point: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}

/** Wait in while-loop for a client connecting to the WiFi **/
void Wait_For_Client_Connection(){
    Serial.println("Waiting for clients to connect!");
    while(WiFi.softAPgetStationNum() <= 0){
        delay(50);
    }
    Serial.println("Client connected! ");
    Serial.println(WiFi.softAPgetStationNum());
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    Connect_To_AP(AP_SSID, WiFi);
    UDP.begin(UDP_PORT);
    Serial.println("Hello there");
}

bool encrypted = false;

void loop(){
    Serial.println();
    String received_message_UDP = Receive_UDP_Packet(UDP, UDP_RECEIVER_BUFFER);

    Serial.print("Received message: ");
    Serial.println(received_message_UDP);

    if (ENABLE_ENCRYPTION){
        int decodedLength = Base64.decodedLength((char *) received_message_UDP.c_str(), (int) received_message_UDP.length());
        char assembledData[decodedLength];
        Base64.decode(
                assembledData,
                (char *) received_message_UDP.c_str(),
                (int) received_message_UDP.length()
        );

        /** Encryption **/
        Serial.println("------------------------------------");

        Serial.print("Response size: ");
        Serial.println(decodedLength);

        uint8_t ciphertext[decodedLength];
        uint8_t buffer[decodedLength];

        for (int index = 0; index < decodedLength; index++)
        {
            buffer[index] = assembledData[index];
        }
        delay(10);
        DisassembleAuthenticaedEncryptionPacket(IV, Tag, 16, ciphertext, buffer, decodedLength);
        delay(10);

        performDecryption(ciphertext, Tag, IV, decodedLength);
        Serial.println();

        memset(buffer, 0, sizeof(buffer));
    }
    else {
        String output_format = "Received UDP packet from ["+UDP.remoteIP().toString()+":"
                               + String(UDP.remotePort()) + "]" + " - Message: ";
        Serial.println(output_format);
        Serial.println(received_message_UDP);
    }
    delay(10);
    memset(UDP_RECEIVER_BUFFER, 0, sizeof(UDP_RECEIVER_BUFFER));

}

