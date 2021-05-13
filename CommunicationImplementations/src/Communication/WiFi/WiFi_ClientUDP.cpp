#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFi_Helper.h>

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
    Build_And_Send_UDP_Packet(UDP, msg, Server_IP, UDP_PORT);
}

