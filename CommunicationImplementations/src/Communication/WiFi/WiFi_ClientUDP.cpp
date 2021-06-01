#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>
#include <utilities.h>
#include <Base64.h>

#define AES_GCM_ENCRYPTION 1
int messageNumber = 0;

WiFiUDP UDP;
IPAddress Server_IP(192,168,43,120);
/** Preparation for encryption **/
uint8_t Tag[16] = {};
uint8_t IV[16] = {};

void setup() {
    Serial.begin(115200);
    Connect_To_AP(AP_SSID, WiFi);
    UDP.begin(UDP_PORT);
}



void loop(){
    delay(5000);
    Serial.println();
    String msg = String("This is an unecrypted UDP message sent via WiFi - Message Number: ") + String(messageNumber++);
    String output_format = "Sending UDP packet to ["+Server_IP.toString()+":" + UDP_PORT + "]" + " - Sent message: ";
    Serial.println(output_format);
    Serial.println(msg);

    if (ENABLE_ENCRYPTION)
    {
        /** Encryption  **/
        int msg_len = (int) msg.length();
        uint8_t plaintext[msg_len];
        uint8_t ciphertext[msg_len];

        // Copy message to plaintext as uint8_t
        charToUint8(msg.c_str(), plaintext, msg_len);

        // Perform encryption
        performEncryption(AES_GCM_ENCRYPTION, plaintext, msg_len, ciphertext, Tag, IV);

        // Concatenate to one big message containing IV + Tag + ciphertext
        uint8_t concatenatedMessage[sizeof(IV) + sizeof (Tag) + sizeof(ciphertext)];
        int concatenatedMessageSize = sizeof(IV) + sizeof (Tag) + sizeof(ciphertext);
        AssembleAuthenticatedEncryptionPacket(IV, Tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

        // Convert to char array as preparation for Base64 encoding
        char assembledCharArray[concatenatedMessageSize];
        uint8ToChar(concatenatedMessage, assembledCharArray, concatenatedMessageSize);

        // Encode Data
        int encodedLength = Base64.encodedLength(concatenatedMessageSize);
        char encoded_content[encodedLength];
        Base64.encode(encoded_content, assembledCharArray, concatenatedMessageSize);

        Build_And_Send_UDP_Packet(UDP, encoded_content, Server_IP, UDP_PORT);

        /** Encryption **/
        Serial.print("Ciphertext: ");
        print_uint8(ciphertext, msg_len);

        Serial.print("Tag: ");
        print_uint8(Tag, 16);

        Serial.print("IV: ");
        print_uint8(IV, 16);
        Serial.println();
    } else
    {
        Build_And_Send_UDP_Packet(UDP, msg.c_str(), Server_IP, UDP_PORT);
    }





}

