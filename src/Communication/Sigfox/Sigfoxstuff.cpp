#include <Arduino.h>
#include <SoftwareSerial.h>

#define SER_RX_PIN D5
#define SER_TX_PIN D6

SoftwareSerial SigfoxSerial(SER_RX_PIN, SER_TX_PIN);

void setup()
{
    Serial.begin(115200);
    SigfoxSerial.begin(9600);
    SigfoxSerial.println("Test");
    Serial.println("HELLO WORLD");

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