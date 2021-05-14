#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>
#include <utilities.h>

#define AES_GCM_ENCRYPTION 1

WiFiUDP UDP;
IPAddress Server_IP(192,168,4,1);

void Connect_To_AP(const char *SSID){
    Serial.println("Connecting to the Access Point: ");
    Serial.println(SSID);

    WiFi.begin(SSID);
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}

void setup() {
    Serial.begin(115200);
    Connect_To_AP(AP_SSID);
    UDP.begin(UDP_PORT);

}
void loop(){
    delay(10000);
    Serial.println();
    const char *msg = "This is an unecrypted UDP message sent via WiFi";
    String output_format = "Sending UDP packet to ["+Server_IP.toString()+":" + UDP_PORT + "]" + " - Sent message: ";
    Serial.println(output_format);
    Serial.println(msg);

    /** Testing  **/
    int msg_len = (int) strlen(msg);
    uint8_t plaintext[msg_len];
    uint8_t ciphertext[msg_len];

    Serial.println("Definition of arrays was successfull");

    // Copy message to plaintext as uint8_t
    charToUint8(msg, plaintext, msg_len);

    Serial.println("Conversion of msg to plaintext was successful");

    // Perform encryption
    performEncryption(AES_GCM_ENCRYPTION, plaintext, msg_len, ciphertext, Tag, IV);

    Serial.println("Encryption passed, and therefore were successful");

    // Concatenate to one big message containing IV + Tag + ciphertext
    uint8_t concatenatedMessage[sizeof(IV) + sizeof (Tag) + sizeof(ciphertext)];
    int concatenatedMessageSize = sizeof(IV) + sizeof (Tag) + sizeof(ciphertext);
    AssembleAuthenticatedEncryptionPacket(IV, Tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

    Serial.println("Assemble function was passed without problems");

    /** Testing **/

    Build_And_Send_UDP_Packet(UDP, concatenatedMessage, concatenatedMessageSize, Server_IP, UDP_PORT);
}

