#include <Arduino.h>
#include <PubSubClient.h>

void msgReceived(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message received on ");
    Serial.print(topic);
    Serial.print(": ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void publisherCheckConnect(PubSubClient &pubSubClient, const char *aws_Endpoint) {
    if (!pubSubClient.connected()) {
        Serial.print("ESP-Publisher connecting to: ");
        Serial.print(aws_Endpoint);
        while (!pubSubClient.connected()) {
            Serial.print(".");
            pubSubClient.connect("ESP-Publisher");
            delay(1000);
        }
        Serial.println("connected");
    }
}

void subscriberCheckConnect(PubSubClient &pubSubClient, const char *aws_Endpoint) {
    if (!pubSubClient.connected()) {
        Serial.print("ESP-Subscriber connecting to: ");
        Serial.print(aws_Endpoint);
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
