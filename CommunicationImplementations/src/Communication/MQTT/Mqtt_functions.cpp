#include <Arduino.h>
#include <PubSubClient.h>

/** Callback function for MQTT_CALLBACK_SIGNATURE **/
void msgReceived(char* topic, byte* payload, unsigned int length)
 {
    Serial.print("Message received on ");
    Serial.print(topic);
    Serial.print(": ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}
/** Check if the Publisher is connected to the Endpoint **/
void publisherCheckConnect(PubSubClient &pubSubClient, const char *endpoint)
{
    if (!pubSubClient.connected()) {
        Serial.print("ESP-Publisher connecting to: ");
        Serial.print(endpoint);
        while (!pubSubClient.connected()) {
            Serial.print(".");
            pubSubClient.connect("ESP-Publisher");
            delay(1000);
        }
        Serial.println("connected");
    }
}

/** Check if the Subscriber is connected to the Endpoint **/
void subscriberCheckConnect(PubSubClient &pubSubClient, const char *endpoint)
{
    if (!pubSubClient.connected()) {
        Serial.print("ESP-Subscriber connecting to: ");
        Serial.print(endpoint);
        while (!pubSubClient.connected()) {
            Serial.print(".");
            pubSubClient.connect("ESP-Subscriber");
            delay(1000);
        }
        Serial.println("connected");
        pubSubClient.subscribe("outTopic");
    }
    pubSubClient.loop();
}
