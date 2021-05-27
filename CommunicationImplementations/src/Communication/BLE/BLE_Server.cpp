#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Arduino.h>
#include <utilities.h>
#include <BLE_Crypto.h>

BLEServer* pServer = nullptr;
BLECharacteristic* pCharacteristic = nullptr;

bool deviceConnected = false;
bool oldDeviceConnected = false;
bool serialMonitorNotified = false;

uint32_t value = 0;

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

bool prepareDecryption = false;
bool ivRecived = false;
bool tagReceived = false;
std::string incomingString;

uint8_t IV[16] = {};
uint8_t Tag[16] = {};
uint8_t ciphertext[20];

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
        if (prepareDecryption){
            incomingString = pCharacteristic->getValue();
            std::string ivTest = incomingString.substr(0, 3);

            if (ivTest == "IV:" || ivRecived ){

                /** Testing **/
                if (!ivRecived){
                    uint8_t *receiver = pCharacteristic->getData();
                    for (int index = 3; index < 19; index++){
                        IV[index - 3] = receiver[index];
                    }

                    Serial.println();
                    Serial.print("Reading value from client: ");
                    Serial.println(incomingString.c_str());
                    Serial.println();
                }
                /** Testing **/

                ivRecived = true;
                std::string tagTest = incomingString.substr(0, 4);

                if (tagTest == "Tag:" || tagReceived){

                    /** Testing **/
                    if (!tagReceived){
                        uint8_t *receiver = pCharacteristic->getData();
                        for (int index = 4; index < 20; index++){
                            Tag[index - 4] = receiver[index];
                        }

                        Serial.println();
                        Serial.print("Reading value from client: ");
                        Serial.println(incomingString.c_str());
                        Serial.println();
                    }
                    /** Testing **/

                    tagReceived = true;
                    if (tagTest != "Tag:" || tagReceived) {
                        for (int index = 0; index < 20; index++){
                            ciphertext[index] = incomingString[index];
                        }
                        Serial.print("Reading value from client: ");
                        Serial.println(incomingString.c_str());

                        Serial.print("Ciphertext: ");
                        print_uint8(ciphertext, (int) incomingString.length());
                        performDecryption(ciphertext, Tag, IV, (int) incomingString.length());
                        Serial.println();
                    }
                }
            }
        }
        // If not encryption
        else {
            incomingString = pCharacteristic->getValue();
            Serial.print("Reading value from client: ");
            Serial.println(incomingString.c_str());
            Serial.println();
        }

        if (incomingString == "Setup encryption")
        {
            pCharacteristic->setValue("Encrypted channel ready");
            pCharacteristic->notify();
            prepareDecryption = true;
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
        pCharacteristic->setValue("You are connected");
        pCharacteristic->notify();
        oldDeviceConnected = deviceConnected;
    }

    delay(3500);
}