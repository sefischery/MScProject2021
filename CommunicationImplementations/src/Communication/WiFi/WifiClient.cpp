#include <WiFi.h>
#include <HTTPClient.h>

const char* SSID = "ESP32-Access-Point";
const char* WiFi_PASS = "123456789";

const char aesResource[] = "http://192.168.4.1/aes";

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

void setup(){
    Serial.begin(115200);

    WiFi.begin(SSID, WiFi_PASS);
    Serial.println("Connecting");
    while(WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    String aescode = httpGETRequest(aesResource);
    Serial.println(aescode);

}

void loop(){
}