#include "WiFi.h"
#include "ESPAsyncWebServer.h"
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

    /** Setup WebServer endpoints **/
    WebServer.on("/aes", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", "Hej med dig, aes");
    });
    WebServer.on("/acorn", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", "Hej med dig, acorn");
    });
    WebServer.on("/ascon", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", "Hej med dig, ascon");
    });

    /** Start the WebServer **/
    WebServer.begin();
}

void loop(){

}