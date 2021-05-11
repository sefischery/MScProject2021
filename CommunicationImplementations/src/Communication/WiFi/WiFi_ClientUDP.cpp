#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>

WiFiUDP UDP;


void Connect_To_AP(const char *SSID){
    Serial.println("Connecting to the Access Point: ");
    Serial.println(SSID);

    // Connect to WiFi
    WiFi.begin(SSID);
    while (WiFiClass::status() != WL_CONNECTED)
    {
        delay(500);
    }

    Serial.println("Connected to WiFi - IP Address: ");
    Serial.print(WiFi.localIP());
    Serial.println(WiFi.macAddress());
}

void setup() {
    Serial.begin(115200);
    UDP.begin(UDP_PORT);
    Connect_To_AP(AP_SSID);
}
void loop(){
    delay(10000);
    Build_And_Send_UDP_Packet(UDP, "test", Server_IP, UDP_PORT);
}

