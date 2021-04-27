/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updated by chegewara

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 4fafc201-1fb5-459e-8fcc-c5c9c331914b
   And has a characteristic of: beb5483e-36e1-4688-b7f5-ea07361b26a8

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   A connect hander associated with the server starts a background task that performs notification
   every couple of seconds.
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <utilities.h>

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

bool deviceConnected = false;
bool oldDeviceConnected = false;
bool serialMonitorNotified = false;

uint32_t value = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* server) override {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* server) override {
        deviceConnected = false;
    }
};



void setup() {
    Serial.begin(115200);

    // Create the BLE Device
    BLEDevice::init("Magnus ESP Server");

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ   |
            BLECharacteristic::PROPERTY_WRITE  |
            BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_BROADCAST
    );

    // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
    // Create a BLE Descriptor
    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");
}

bool encryptionFlag = false;
bool ivRecived = false;
bool tagReceived = false;
std::string incomingString;

uint8_t iv[16] = {};
uint8_t tag[16] = {};
uint8_t ciphertext[20];

uint8_t testingContainer[20];

void loop() {
    // notify changed value
    if (deviceConnected) {
        pCharacteristic->notify();
        pCharacteristic->setBroadcastProperty(true);
        if (!serialMonitorNotified){
            Serial.println("Client connected...");
            serialMonitorNotified = true;
        }

        //If the encryption; This needs to be fixed, ugly as fuck!
        if (encryptionFlag){
            incomingString = pCharacteristic->getValue();
            std::string ivTest = incomingString.substr(0, 3);

            if (ivTest == "iv:" || ivRecived ){

                /** Testing **/
                if (!ivRecived){
                    uint8_t *receiver = pCharacteristic->getData();
                    for (int index = 3; index < 19; index++){
                        iv[index-3] = receiver[index];
                    }
                }
                /** Testing **/

                ivRecived = true;
                std::string tagTest = incomingString.substr(0, 4);

                if (tagTest == "tag:" || tagReceived){

                    /** Testing **/
                    if (!tagReceived){
                        uint8_t *receiver = pCharacteristic->getData();
                        for (int index = 4; index < 20; index++){
                            tag[index-4] = receiver[index];
                        }
                    }
                    /** Testing **/

                    tagReceived = true;
                    if (tagTest != "tag:" || tagReceived) {
                        for (int index = 0; index < 20; index++){
                            ciphertext[index] = incomingString[index];
                        }
                        Serial.print("Ciphertext: ");
                        print_uint8(ciphertext, 20);
                        Serial.print("Tag: ");
                        print_uint8(tag, 16);
                        Serial.print("Iv: ");
                        print_uint8(iv, 16);
                        performDecryption(ciphertext, tag, iv);
                        Serial.println();
                    }
                }
            }
        }
        // If not encryption
        else {
            //  uint8_t *hi = pCharacteristic->toString();
            incomingString = pCharacteristic->getValue();
            Serial.println(incomingString.c_str());
        }

        if (incomingString == "Setup encryption")
        {
            pCharacteristic->setValue("Server: Encrypted channel ready");
            pCharacteristic->notify();
            encryptionFlag = true;

        }

        delay(1000); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
        pCharacteristic->setBroadcastProperty(false);

        if (serialMonitorNotified){
            Serial.println("Client disconnected...");
            serialMonitorNotified = false;
        }
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        pCharacteristic->setValue("Server: You are connected");
        pCharacteristic->notify();
        oldDeviceConnected = deviceConnected;
    }

    delay(2000);
}