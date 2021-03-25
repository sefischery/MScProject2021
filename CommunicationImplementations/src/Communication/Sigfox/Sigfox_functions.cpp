#include <SoftwareSerial.h>

void sendSigfoxATCommand(char const *Command, SoftwareSerial &softwareSerial)
{
    softwareSerial.print(Command);
}

//Send Sigfox Message
void sendSigfoxMessage(uint8_t *msg, int size, SoftwareSerial &softwareSerial)
{
    Serial.println("Inside sendSigfoxMessage");

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
    Serial.println(sigfoxCommand);
    softwareSerial.println(sigfoxCommand);

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