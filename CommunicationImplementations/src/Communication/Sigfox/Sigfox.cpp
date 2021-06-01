#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Sigfox_functions.h>
#include <utilities.h>
#include <Base64.h>

SoftwareSerial SigfoxSerial(SER_RX_PIN, SER_TX_PIN);

int messageCount = 0;
void setup()
{
    delay(2000);
    Serial.begin(115200);
    SigfoxSerial.begin(9600);
    Serial.println("Connection established to the Sigfox module");

    //uint8_t msg[12] = "magnussebas";
    //sendSigfoxMessage(msg, sizeof(msg), SigfoxSerial);

    /** Encryption **/
    // Set message
    String message = String("Message: ") + ++messageCount;

    Serial.print("Plain data: \"");Serial.print(message);Serial.println("\"");

    // Define arrays
    uint8_t iv[16];
    uint8_t tag[16];
    uint8_t plaintext[message.length()];
    uint8_t ciphertext[message.length()];

    // Copy message to plaintext as uint8_t
    charToUint8(message.c_str(), plaintext, (int) message.length());

    // Perform encryption
    performEncryption(ACORN_ENCRYPTION, plaintext, (int) message.length(), ciphertext, tag, iv);

    // Concatenate to one big message containing IV + Tag + ciphertext
    int concatenatedMessageSize = 32 + message.length();
    uint8_t concatenatedMessage[concatenatedMessageSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, 16, ciphertext, concatenatedMessage, concatenatedMessageSize);

    Serial.println();
    Serial.print("Encrypted data: ");print_uint8(concatenatedMessage, concatenatedMessageSize);
    Serial.println();

    // Convert to char array for base64 encoding
    char assembledCharArray[concatenatedMessageSize];
    uint8ToChar(concatenatedMessage, assembledCharArray, concatenatedMessageSize);

    // Needs to be changed to base64
    int encodedLength = Base64.encodedLength(concatenatedMessageSize);
    char encoded_content[encodedLength];
    Base64.encode(encoded_content, assembledCharArray, concatenatedMessageSize);

    // Helping print statements
    Serial.print("Complete packet size: ");Serial.println(encodedLength);
    Serial.print("Divided into: ");Serial.print(encodedLength / 12);Serial.print(".");Serial.print(encodedLength % 12);Serial.println(" number of packages.");
    Serial.println();
    Serial.print("Base64 encoded data: ");
    print_char(encoded_content, encodedLength);
    Serial.println();

    /** TEMP **/
    uint8_t transmittingMessage1[12];
    int transmissionCount1 = 0;
    for (int index = 0; index < encodedLength; ++index) {
        if (((index % 12) == 0) && ((index / 12) > 0))
        {
            Serial.print("Packet ");Serial.print(transmissionCount1);Serial.print(": ");
            print_uint8(transmittingMessage1, 12);
            transmissionCount1++;
        }

        transmittingMessage1[index % 12] = encoded_content[index];
    }

    if (encodedLength % 12 != 0)
    {
        uint8_t remainingData[encodedLength % 12];
        for (int index = 0; index < encodedLength % 12; ++index) {
            remainingData[index] = transmittingMessage1[index];
        }

        Serial.print("Packet 4: ");
        print_uint8(remainingData, encodedLength % 12);
        Serial.println();
    }
    /** TEMP **/

    // Prepare message transmission with max length of 12
    uint8_t transmittingMessage[12];
    int transmissionCount = 0;
    for (int index = 0; index < encodedLength; ++index) {
        if (((index % 12) == 0) && ((index / 12) > 0))
        {
            delay(10);
            sendSigfoxMessage(transmittingMessage, 12, SigfoxSerial);
            delay(10);
            Serial.print("Transmission, number: ");Serial.print(transmissionCount++);Serial.println(", was successful.");
            Serial.print("Message contained: ");
            print_uint8(transmittingMessage, 12);
            Serial.println();
        }

        transmittingMessage[index % 12] = encoded_content[index];
    }

    if (encodedLength % 12 != 0)
    {
        uint8_t remainingData[encodedLength % 12];
        for (int index = 0; index < encodedLength % 12; ++index) {
            remainingData[index] = transmittingMessage[index];
        }

        delay(10);
        sendSigfoxMessage(remainingData, encodedLength % 12, SigfoxSerial);
        delay(10);
        Serial.print("Transmission, number: ");Serial.print(transmissionCount++);Serial.println(", was successful.");
        Serial.print("Message contained: ");
        print_uint8(remainingData, encodedLength % 12);
        Serial.println();
    }
    /** Encryption **/
}

void loop()
{
    // This is used if AT commands are needed.
    /*
    delay(1000);
    String pac = "";
    char output;

    sendSigfoxATCommand(AT_PAC_COMMAND,SigfoxSerial);
    delay(1000);
    while(SigfoxSerial.available()){
        output = SigfoxSerial.read();
        pac += output;
        delay(10);
    }

    Serial.println("---------------");
    Serial.println("PAC number: ");
    Serial.println(pac);
    Serial.println("---------------");
    delay(1000);
    */
}
