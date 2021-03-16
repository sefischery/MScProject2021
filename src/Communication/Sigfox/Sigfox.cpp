#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial SigfoxSerial(SER_RX_PIN, SER_TX_PIN);

//Send Sigfox Message
void sendMessage(uint8_t msg[], int size)
{
    Serial.println("Inside sendMessage");

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
    SigfoxSerial.println(sigfoxCommand);

    while (!SigfoxSerial.available())
    {
        Serial.println("Waiting for response");
        delay(1000);
    }

    while (SigfoxSerial.available())
    {
        output = (char)SigfoxSerial.read();
        status += output;
        delay(10);
    }

    Serial.println();
    Serial.print("Status \t");
    Serial.println(status);
}



void setup()
{
    Serial.begin(115200);
    SigfoxSerial.begin(9600);
    SigfoxSerial.println("Test");
    Serial.println("HELLO WORLD");
    uint8_t msg[12] = "magnussebas";
    sendMessage(msg, sizeof(msg));

}

void loop()
{
    delay(1000);
    String pac = "";
    char output;


    SigfoxSerial.print("AT$I=11\r");
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
}