#include "WiFi.h"
#include "WiFiUdp.h"

WiFiUDP UDP;
//WiFi information for the setup.
#define AP_SSID "ESP32SOFTAP"
#define AP_CHANNEL 1
#define AP_CLOACK 0
#define AP_MAX_CON 5
#define UDP_PORT 1500
//#define AP_PASSWORD ""

//IP-addresses declarations.
IPAddress Server_IP(192,168,10,8);
IPAddress Client_IP(192,168,10,9);

void Create_Open_AccessPoint(const char *SSID, int WiFi_Channel, int Hide_Access_Point, int Max_Number_of_Connections) {
    WiFi.softAP(SSID,nullptr,WiFi_Channel,Hide_Access_Point,Max_Number_of_Connections);
    Serial.println("Assigned IP address of Access Point: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC address: ");
    Serial.println(WiFi.macAddress());
}

/** Wait in while-loop for a client connecting to the WiFi **/

void Wait_For_Client_Connection(){
    while(WiFi.softAPgetStationNum() <= 0){
        delay(1);
    }
    Serial.println("Client connected! ");
    Serial.println(WiFi.softAPgetStationNum());
    Serial.println();
}

void Build_And_Send_UDP_Packet(){
    Serial.println("()");
    UDP.beginPacket(Client_IP, UDP_PORT);
    UDP.print("Hej");
    UDP.endPacket();
}

void setup() {
    Serial.begin(115200);
    Create_Open_AccessPoint(AP_SSID, AP_CHANNEL, AP_CLOACK, AP_MAX_CON);
    UDP.begin(UDP_PORT);
    Wait_For_Client_Connection();
    Build_And_Send_UDP_Packet();

}
void loop(){

}

