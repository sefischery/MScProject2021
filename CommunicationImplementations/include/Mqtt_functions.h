#ifndef MSCPROJECT2021_MQTT_FUNCTIONS_H
#define MSCPROJECT2021_MQTT_FUNCTIONS_H

#define SSID "Testfi"
#define WiFi_PASS "Ok123456"
#define awsEndpoint "a3pe5ezmacr7yu-ats.iot.eu-west-1.amazonaws.com"

void msgReceived(char* topic, byte* payload, unsigned int len);
void publisherCheckConnect(PubSubClient &pubSubClient, const char* aws_Endpoint);
void subscriberCheckConnect(PubSubClient &pubSubClient, const char* aws_Endpoint);
#endif //MSCPROJECT2021_MQTT_FUNCTIONS_H
