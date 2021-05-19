#include <SoftwareSerial.h>
#include <utilities.h>
#include <Encryption_testing.h>

/** Sends an inputted AT command to the Sigfox module via SoftwareSerial **/
void sendSigfoxATCommand(char const *Command, SoftwareSerial &softwareSerial)
{
    softwareSerial.print(Command);
}

/** Sends a message to the Sigfox backend while also waiting for response **/
void sendSigfoxMessage(uint8_t *msg, int size, SoftwareSerial &softwareSerial)
{
    Serial.println("Inside sendSigfoxMessage");
    delay(2000);
    String status = "";
    String hexChar = "";
    String sigfoxCommand = "";
    char output;

    sigfoxCommand += "AT$SF=";

    for (int i = 0; i < size; i++)
    {
        hexChar = String(msg[i], HEX);

        //padding
        if (hexChar.length() == 1)
        {
            hexChar = "0" + hexChar;
        }

        sigfoxCommand += hexChar;
    }

    Serial.println("Sending...");
    //Serial.println(sigfoxCommand);
    softwareSerial.println(sigfoxCommand);
    delay(5000);
    while (!softwareSerial.available())
    {
        Serial.println("Waiting for response");
        delay(1000);
    }

    while (softwareSerial.available())
    {
        output = (char)softwareSerial.read();
        status += output;
        delay(10);
    }

    Serial.println();
    Serial.print("Status \t");
    Serial.println(status);
}

void performEncryption(int encryptionType, uint8_t *plaintext, int inputSize,
                       uint8_t *ciphertextReceiver, uint8_t *Tag, uint8_t *IV) {
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
