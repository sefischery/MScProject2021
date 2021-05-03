#include <Arduino.h>
#include <aes_gcm.h>
#include <ascon.h>
#include <utilities.h>
#include <acorn.h>
#include <Crypto.h>
#include <Encryption_testing.h>

#define SIZE 16

void setup() {
    Serial.begin(115200);

    unsigned long toEnd;
    unsigned long start = micros();

    /** Define initial text **/
    int randomNumber = random(0,15);
    int messageSize = 128;
    //uint8_t inputUint8[messageSize];
    char input[] = "hej med dig min ven";
    //GenerateInitializationVector(inputUint8, messageSize);
    //uint8ToChar(inputUint8, input, messageSize);
    Serial.println();
    Serial.println("----------------------------------------------------");
    Serial.println("-----------------------Sender-----------------------");
    Serial.println("----------------------------------------------------\n");

    Serial.println("Step 1. Initialize input");
    Serial.print("Input message: ");
    print_char(input, 19);
    Serial.println();

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

    Serial.println("Step 2. Define arrays: tag, iv, plaintext & ciphertext\n");

    /** IV initialization **/
    GenerateInitializationVector(iv, SIZE);

    Serial.println("Step 3. Generate random initialization vector");
    Serial.print("Iv: ");
    print_uint8(iv, SIZE);
    Serial.println();

    /** Encryption **/
    cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE, false);

    Serial.println("Step 4. Compute tag");
    Serial.print("Tag: ");
    print_uint8(tag, SIZE);
    Serial.println();

    Serial.println("Step 5. Perform encryption and return result in ciphertext array");
    Serial.print("Ciphertext: ");
    print_uint8(ciphertextReceiver, 19);
    Serial.println();

    /** Manipulate packet **/
    //ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/

    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);

    Serial.println("Step 6. Assemble IV, Tag & ciphertext into one packet");
    Serial.print("Assembled data array: ");
    print_uint8(packetBuffer, packetSize);
    Serial.println();

    Serial.println("Step 7. Send assembled data\n");

    Serial.println("----------------------------------------------------");
    Serial.println("----------------------Receiver----------------------");
    Serial.println("----------------------------------------------------\n");

    Serial.println("Step 1. Define arrays: tag, iv, plaintext & ciphertext\n");

    Serial.println("Step 2. Retrieve data as one big packet");
    Serial.print("Received data: ");
    print_uint8(packetBuffer, packetSize);
    Serial.println();

    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    Serial.println("Step 3. Disassemble data in to IV, Tag and Ciphertext array");
    Serial.print("Iv: ");
    print_uint8(iv, SIZE);
    Serial.print("Tag: ");
    print_uint8(tag, SIZE);
    Serial.print("Ciphertext: ");
    print_uint8(ciphertextReceiver, 19);
    Serial.println();

    /** Decryption **/
    bool decryptionValidation = cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE, false);
    Serial.println("Step 4. Perform decryption of ciphertext");
    Serial.print("Plaintext: ");
    print_uint8(plaintextReceiver, 19);
    Serial.println();

    Serial.println("Step 5. Compute tag and validate tag to ensure integrity");
    Serial.print("Validity of tag: ");
    if (decryptionValidation){
        Serial.println("Approved");
    } else {
        Serial.println("Denied");
    }
    Serial.println();

    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    Serial.println("Step 6. Convert plaintext to human readable content");
    Serial.print("Decrypted message: ");
    print_char(text, textSize);
    Serial.println();

    Serial.println("Sequence finished");
}

void loop() {
}