#include <Arduino.h>
#include <aes_gcm.h>
#include <ascon.h>
#include <utilities.h>
#include <acorn.h>
#include <Crypto.h>

#define SIZE 16

struct encryption {
    bool (*acorn_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*aes_gcm_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*ascon_encryption)(uint8_t *plaintext, uint8_t *ciphertext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
};

struct decryption {
    bool (*acorn_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*aes_gcm_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
    bool (*ascon_decryption)(uint8_t *ciphertext, uint8_t *plaintext, uint8_t *tag, int size, uint8_t *key, uint8_t *iv, int cipherSize);
};

struct cipherOperator {
    struct encryption encryption;
    struct decryption decryption;
    uint8_t key[SIZE];
};



void setup() {
    Serial.begin(115200);

    unsigned long toEnd;
    unsigned long start = micros();

    struct cipherOperator cipher = {
            {
                    /** Defined encryption methods **/
                    acorn_encryption,
                    aes_gcm_encryption,
                    ascon_encryption
            },{
                    /** Defined decryption methods **/
                    acorn_decryption,
                    aes_gcm_decryption,
                    ascon_decryption
            },{
                    /** Key definition **/
                    0x00, 0x01, 0x02, 0x03, 0x04,
                    0x05, 0x06, 0x07, 0x08, 0x09,
                    0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
                    0x0f
            }
    };

    /** Define initial text **/
    int randomNumber = random(0,15);
    int messageSize = 128;
    uint8_t inputUint8[messageSize];
    char input[messageSize]; // ] = "hej med dig min ven";
    GenerateInitializationVector(inputUint8, messageSize);
    uint8ToChar(inputUint8, input, messageSize);

    Serial.println(messageSize);

    /** Allow the plaintext to vary in size **/
    const int textSize = sizeof(input);

    /** Setup for readable text to plaintext **/
    char text[textSize];

    /** Convert char array to plaintext format; uint8_t **/
    uint8_t plaintext[textSize];
    charToUint8(input, plaintext, textSize);

    /** Validation process **/
    uint8_t tag[SIZE];
    uint8_t iv[SIZE];
    uint8_t plaintextReceiver[textSize];
    uint8_t ciphertextReceiver[textSize];

    /** IV initialization **/
    GenerateInitializationVector(iv, SIZE);

    /** Encryption **/
    cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);

    /** Manipulate packet **/
    //ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/

    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    loadPacketBuffer(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);

    /** Receiver: Unpack received data into iv, tag and informative data **/
    separatePacketBuffer(packetBuffer, packetSize, iv, tag, SIZE, ciphertextReceiver);

    /** Decryption **/
    bool decryptionValidation = cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);

    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);

    /** Printing functionality sender **/
    Serial.println("----------------------------------------------------");
    Serial.println("----------------------Receiver----------------------");
    Serial.println("----------------------------------------------------");
    Serial.print("Message: ");
    print_char(input, textSize);
    Serial.println("                         |----16 Bytes---|---16 Bytes---|--- Varies ---| ");
    Serial.println("The encapsulated packet: |      Iv       |     Tag      |  Ciphertext  |");
    print_uint8(packetBuffer, packetSize);

    /** Print functionality receiver **/
    Serial.println("----------------------------------------------------");
    Serial.println("-----------------------Sender-----------------------");
    Serial.println("----------------------------------------------------");
    Serial.print("IV: ");
    print_uint8(iv, SIZE);
    Serial.print("Tag: ");
    print_uint8(tag, SIZE);
    Serial.print("Ciphertext: ");
    print_uint8(ciphertextReceiver, packetSize - 2 * SIZE);
    Serial.print("Tag validation: ");
    Serial.println(decryptionValidation);
    Serial.print("Decrypted message: ");
    print_char(text, textSize);
    Serial.println("----------------------------------------------------");


    toEnd = micros() - start;
    Serial.print(toEnd);
    Serial.println(" Micro seconds from start to end");
}

void loop() {
}