#include <Arduino.h>
#include <PubSubClient.h>
#include <MQTT_functions.h>
#include <Encryption_testing.h>
#include <ArduinoJson.h>
#include <Base64.h>

#define SIZE_16 16

/** Callback function for MQTT_CALLBACK_SIGNATURE **/
void  msgReceived(char* topic, byte* payload, unsigned int length)
{
    /** Testing **/
    // Assign the received data to uint8_t type
    char associatedMessage[length];
    for (int index = 0; index < length; index++)
    {
        associatedMessage[index] = (char)payload[index];
    }

    Serial.print("Message received on ");
    Serial.print(topic);
    Serial.print(": ");
    print_char(associatedMessage, length);

    uint8_t IV[SIZE_16];
    uint8_t Tag[SIZE_16];
    uint8_t ciphertext[SIZE_16];

    DynamicJsonDocument contentObject(512);
    deserializeJson(contentObject, associatedMessage);

    const char *constContent = contentObject["content"];
    const int encoded_payload_size = contentObject["encode-content-size"];
    const int payload_size = contentObject["payload-size"];

    char content[encoded_payload_size];
    for (int index = 0; index < encoded_payload_size; index++)
    {
        content[index] = constContent[index];
    }

    char assembledData[payload_size];
    Base64.decode(assembledData, content, encoded_payload_size);

    uint8_t assembledDataHex[payload_size];
    for (int index = 0; index < payload_size; index++)
    {
        assembledDataHex[index] = assembledData[index];
    }

    DisassembleAuthenticaedEncryptionPacket(IV, Tag, 16, ciphertext, assembledDataHex, payload_size);

    performDecryption(ciphertext, Tag, IV, payload_size - 32);

    Serial.println("---------------------------------------------");
    /** Testing **/

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
        pubSubClient.subscribe("Encryption");
    }
    pubSubClient.loop();
}

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize, uint8_t *ciphertextReceiver,
                       uint8_t *Tag, uint8_t *IV) {
    /** IV initialization **/
    GenerateInitializationVector(IV, 16);

    /** Perform encryption and timings **/
    if (encryptionType == 1)
    {
        cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 2)
    {
        cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
    else if (encryptionType == 3)
    {
        cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, Tag, inputSize, cipher.key, IV, false);
    }
}

void performDecryption(uint8_t *ciphertext, uint8_t *Tag, uint8_t *IV, int ciphertextSize) {
    /** Plaintext buffer **/
    uint8_t plaintextReceiver[ciphertextSize];
    char text[ciphertextSize];

    /** Perform decryption and timing of the following **/
    cipher.decryption.aes_gcm_decryption(ciphertext, plaintextReceiver, Tag, ciphertextSize, cipher.key, IV, false);

    Serial.print("Tag: ");
    print_uint8(Tag, 16);

    Serial.print("IV: ");
    print_uint8(IV, 16);

    Serial.print("Ciphertext: ");
    print_uint8(ciphertext, ciphertextSize);

    uint8ToChar(plaintextReceiver, text, ciphertextSize);
    Serial.print("Decrypted Text: ");
    print_char(text, ciphertextSize);
}