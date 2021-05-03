#include <TransistorNoiseSource.h>
#include <RNG.h>
#include <utilities.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "WiFi_Helper.h"

#define WebServerPort 80

/** Network Credentials **/
const char* SSID = "ESP32-Access-Point";
const char* WiFi_PASS = "123456789";

AsyncWebServer WebServer(WebServerPort);


void setup(){
    Serial.begin(115200);
    Serial.println();
    Serial.print("Setting AP (Access Point)…");
    /** Initialize the WiFi Access Point **/
    WiFi.softAP(SSID, WiFi_PASS);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("Access Point -> IP address: ");
    Serial.println(IP);

    /** Define Encryption Variables **/
    uint8_t plaintext[messageSize];
    charToUint8(message, plaintext, messageSize);

    /** Setup WebServer endpoints **/

    /** AES Setup **/
    performEncryption(1, plaintext, messageSize, aesCiphertext, tag, iv);
    uint8_t aesConcatenatedMessage[sizeof(iv) + sizeof (tag) + sizeof(aesCiphertext)];
    int aesConcatenatedMessageSize = sizeof(iv) + sizeof (tag) + sizeof(aesCiphertext);
    AssembleAuthenticatedEncryptionPacket(iv, tag, 16, aesCiphertext, aesConcatenatedMessage, aesConcatenatedMessageSize);

    Serial.print("Tag: ");
    print_uint8(tag, 16);

    Serial.print("Iv: ");
    print_uint8(iv, 16);

    Serial.print("Concatenated message: ");
    print_uint8(aesConcatenatedMessage, aesConcatenatedMessageSize);

    /** AES Setup **/

    WebServer.on("/aes", HTTP_GET, [aesConcatenatedMessage, aesConcatenatedMessageSize](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", aesConcatenatedMessage, aesConcatenatedMessageSize);
    });

    /** Acorn Setup **/
    performEncryption(2, plaintext, messageSize, acornCiphertext, tag, iv);
    uint8_t acornConcatenatedMessage[sizeof(iv) + sizeof (tag) + sizeof(acornCiphertext)];
    int acornConcatenatedMessageSize = sizeof(iv) + sizeof (tag) + sizeof(acornCiphertext);
    AssembleAuthenticatedEncryptionPacket(iv, tag, 16, acornCiphertext, acornConcatenatedMessage, acornConcatenatedMessageSize);
    /** Acorn Setup **/

    WebServer.on("/acorn", HTTP_GET, [acornConcatenatedMessage, acornConcatenatedMessageSize](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", acornConcatenatedMessage, acornConcatenatedMessageSize);
    });

    /** Ascon Setup **/
    performEncryption(3, plaintext, messageSize, asconCiphertext, tag, iv);
    uint8_t asconConcatenatedMessage[sizeof(iv) + sizeof (tag) + sizeof(asconCiphertext)];
    int asconConcatenatedMessageSize = sizeof(iv) + sizeof (tag) + sizeof(asconCiphertext);
    AssembleAuthenticatedEncryptionPacket(iv, tag, 16, asconCiphertext, asconConcatenatedMessage, asconConcatenatedMessageSize);
    /** Ascon Setup **/

    WebServer.on("/ascon", HTTP_GET, [asconConcatenatedMessage, asconConcatenatedMessageSize](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", asconConcatenatedMessage, asconConcatenatedMessageSize);
    });

    /** Unencrypted endpoint **/
    WebServer.on("/clear-text", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", "Hej med dig");
    });

    /** Start the WebServer **/
    WebServer.begin();
}

void loop(){

}