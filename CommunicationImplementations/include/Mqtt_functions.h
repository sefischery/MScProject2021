#ifndef MSCPROJECT2021_MQTT_FUNCTIONS_H
#define MSCPROJECT2021_MQTT_FUNCTIONS_H

#define SSID "Testfi"
#define WiFi_PASS "Ok123456"
#define AWS_ENDPOINT "a3pe5ezmacr7yu-ats.iot.eu-west-1.amazonaws.com"
#define MQTT_PORT 8883
#define BAUD_RATE 115200

void msgReceived(char* topic, byte* payload, unsigned int len);
void publisherCheckConnect(PubSubClient &pubSubClient, const char* endpoint);
void subscriberCheckConnect(PubSubClient &pubSubClient, const char* endpoint);
#endif //MSCPROJECT2021_MQTT_FUNCTIONS_H
