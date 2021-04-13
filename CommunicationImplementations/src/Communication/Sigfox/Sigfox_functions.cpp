#include <SoftwareSerial.h>

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