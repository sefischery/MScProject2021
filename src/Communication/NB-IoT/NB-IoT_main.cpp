#include <Arduino.h>
#define DEBUG_STREAM SerialUSB
#define MODEM_STREAM Serial1
#define powerPin SARA_ENABLE
#define voltagePin SARA_R4XX_TOGGLE

unsigned long baud = 115200;

void setup()
{

    // Put voltage on the nb-iot module
    pinMode(powerPin, OUTPUT);
    digitalWrite(powerPin, HIGH);
    pinMode(voltagePin, OUTPUT);
    digitalWrite(voltagePin, LOW);

    // Start communication
    DEBUG_STREAM.begin(baud);
    MODEM_STREAM.begin(baud);
}

// Forward every message to the other serial
void loop() {
    MODEM_STREAM.write("AT+CSQ\r");

    while (MODEM_STREAM.available()) {
        DEBUG_STREAM.write(MODEM_STREAM.read());
    }


    delay(5000);
}