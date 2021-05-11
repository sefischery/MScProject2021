#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Sigfox_functions.h>

SoftwareSerial SigfoxSerial(SER_RX_PIN, SER_TX_PIN);

int messageCount = 0;
void setup()
{
    Serial.begin(115200);
    SigfoxSerial.begin(9600);
    SigfoxSerial.println("Test");
    Serial.println("HELLO WORLD");
    uint8_t msg[12] = "magnussebas";
    sendSigfoxMessage(msg, sizeof(msg), SigfoxSerial);

    /** Testing **/
    // Array definitions
    uint8_t firstMessage[12] = {0x00};
    uint8_t secondMessage[12] = {0x00};
    uint8_t thirdMessage[12] = {0x00};
    uint8_t data[12];

    // Set message
    String message = String("Message: ") + ++messageCount;

    // Define operating arrays
    uint8_t tag[16];
    uint8_t iv[16];
    uint8 plaintext[message.length()];
    uint8_t ciphertext[message.length()];

    // Fill plaintext array with message
    for (int index = 0; index < message.length(); index++)
    {
        plaintext[index] = message[index];
    }

    // Perform encryption, thereby also fill iv and tag
    performEncryption(1, plaintext, (int) msg.length(), ciphertext, tag, iv);







    /** Testing **/
}

void loop()
{
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
