#include <Arduino.h>
#include <BLEDevice.h>
#include <BLE_Crypto.h>
#include <utilities.h>

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;

static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;
static BLEScan* pBLEScan;
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");


class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pClient) override {
    }

    void onDisconnect(BLEClient* pClient) override {
        connected = false;
        Serial.println("onDisconnect");
    }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient *pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
        Serial.print("Failed to find our service UUID: ");
        Serial.println(serviceUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our service");


    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
        Serial.print("Failed to find our characteristic UUID: ");
        Serial.println(charUUID.toString().c_str());
        pClient->disconnect();
        return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
        std::string value = pRemoteCharacteristic->readValue();
        Serial.print("The characteristic value was: ");
        Serial.println(value.c_str());
    }

    connected = true;

    return true;
}
/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
      * Called for each advertising BLE server.
      */
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        Serial.print("BLE Advertised Device found: ");
        Serial.println(advertisedDevice.toString().c_str());

        // We have found a device, let us now see if it contains the service we are looking for.
        if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

            BLEDevice::getScan()->stop();
            myDevice = new BLEAdvertisedDevice(advertisedDevice);
            doConnect = true;
            doScan = true;

        } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks


void BLEClientSetup(){
    BLEDevice::init("EspMagnusClient");
    // Retrieve a Scanner and set the callback we want to use to be informed when we
    // have detected a new device.  Specify that we want active scanning and start the
    // scan to run for 5 seconds.
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

bool performServerConnectionAttempt(){
    bool connectionAttempt = connectToServer();
    if (connectionAttempt) {
        Serial.println("Connection to BLE Server: ESTABLISHED");
        return false;
    } else {
        Serial.println("Connection to BLE Server: FAILED");
        return true;
    }
}

bool encryptionPerformed = false;
bool hasSentIv = false;
bool hasSentTag = false;
bool sendingEncryptedText = false;

uint8_t IV[16] = {0};
uint8_t Tag[16] = {0};

uint8_t copyOfCipherText[20] = {0};

int msgNumber = 0;

void changeBleServerCharacteristics(){
    std::string value = pRemoteCharacteristic->readValue();
    Serial.println(value.c_str());

    if (ENABLE_ENCRYPTION)
    {
        if (value == "Server: You are connected")
        {
            pRemoteCharacteristic->writeValue("Setup encryption");
        }
        else if (value == "Server: Encrypted channel ready" || sendingEncryptedText)
        {
            sendingEncryptedText = true;

            /** Define initial text **/
            char message[] = "Secret: 1234";
            const int textSize = sizeof(message);

            if(!encryptionPerformed){
                uint8_t plaintext[textSize];
                charToUint8(message, plaintext, textSize);
                uint8_t ciphertextReceiver[textSize];

                performEncryption(AES_GCM_ENCRYPTION, plaintext, textSize, ciphertextReceiver, Tag, IV);
                encryptionPerformed = true;
                copy_uint8(ciphertextReceiver, copyOfCipherText, textSize);
                Serial.print("Ciphertext: ");
                print_uint8(ciphertextReceiver, 20);
                Serial.print("Tag: ");
                print_uint8(Tag, 16);
                Serial.print("Iv: ");
                print_uint8(IV, 16);
            }

            if (!hasSentIv){
                hasSentIv = true;
                uint8_t writer[19] = "IV:";

                for (int index = 3; index < 19; index++)
                {
                    writer[index] = IV[index - 3];
                }
                Serial.print("writing IV: ");
                print_uint8(writer, 19);
                pRemoteCharacteristic->writeValue(writer, 19);
            }
            else if (!hasSentTag){
                hasSentTag = true;
                pRemoteCharacteristic->writeValue("Tag:");

                uint8_t writer[20] = "Tag:";

                for (int index = 4; index < 20; index++)
                {
                    writer[index] = Tag[index - 4];
                }

                Serial.print("writing Tag: ");
                print_uint8(Tag, 20);

                pRemoteCharacteristic->writeValue(writer, 20);

            }
            else {
                pRemoteCharacteristic->writeValue(copyOfCipherText, textSize);
            }
        }
    }
    else {
        String msg = String("Unencrypted msg: ") + msgNumber++;
        pRemoteCharacteristic->writeValue(msg.c_str());
    }

}


void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE Client...");
    BLEClientSetup();
}

void loop() {
    if (doConnect) {
        doConnect = performServerConnectionAttempt();
    }

    if (connected) {
        changeBleServerCharacteristics();
    }
    else if (doScan) {
        // Consider moving this to a method.
        BLEDevice::getScan()->start(0);
    }

    delay(5000);
}