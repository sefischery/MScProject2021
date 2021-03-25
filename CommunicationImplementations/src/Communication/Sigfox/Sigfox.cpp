#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Sigfox_functions.h>

SoftwareSerial SigfoxSerial(SER_RX_PIN, SER_TX_PIN);

void setup()
{
    Serial.begin(115200);
    SigfoxSerial.begin(9600);
    SigfoxSerial.println("Test");
    Serial.println("HELLO WORLD");
    uint8_t msg[12] = "magnussebas";
    sendSigfoxMessage(msg, sizeof(msg), SigfoxSerial);

}

void loop()
{
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
}
