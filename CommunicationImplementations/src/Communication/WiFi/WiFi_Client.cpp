#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFi_Helper.h>
#include <utilities.h>

/** Network Credentials **/
const char* SSID = "ESP32-Access-Point";
const char* WiFi_PASS = "123456789";

const char webserver[] = "http://192.168.4.1/aes";

String httpGETRequest(const char* serverName) {
    HTTPClient http;

    http.begin(serverName);
    int httpResponseCode = http.GET();

    String payload = "--";

    if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    http.end();

    return payload;
}

byte mac[6];

void setup(){
    Serial.begin(115200);
    /** Connect to the WiFi Access Point **/
    WiFi.begin(SSID);//, WiFi_PASS);
    Serial.println("Connecting");
    while(WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    /** print Mac adresse **/
    WiFi.macAddress(mac);
    Serial.print("MAC: ");
    Serial.print(mac[5],HEX);
    Serial.print(":");
    Serial.print(mac[4],HEX);
    Serial.print(":");
    Serial.print(mac[3],HEX);
    Serial.print(":");
    Serial.print(mac[2],HEX);
    Serial.print(":");
    Serial.print(mac[1],HEX);
    Serial.print(":");
    Serial.println(mac[0],HEX);
    /** print Mac adresse **/

    String response = httpGETRequest(webserver);
    Serial.print("Untreated content: ");
    Serial.println(response);

    /** Testing **/
    int responseSize = (int) response.length();

    Serial.print("Response size: ");
    Serial.println(responseSize);

    uint8_t ciphertext[responseSize];
    uint8_t buffer[responseSize];
    for (int index = 0; index < responseSize; index++)
    {
        buffer[index] = response[index];
    }
    delay(10);
    DisassembleAuthenticaedEncryptionPacket(IV, Tag, 16, ciphertext, buffer, responseSize);
    delay(10);
    performDecryption(ciphertext, Tag, IV, responseSize);

    /** Testing **/
}

void loop(){
}