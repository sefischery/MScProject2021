#include <Arduino.h>
#include <aes_gcm.h>
#include <ascon.h>
#include <utilities.h>
#include <acorn.h>
#include <Crypto.h>
#include <Encryption_testing.h>

#define SIZE_16 16

void setup() {
    Serial.begin(115200);
    /** Define plaintext **/
    char plaintext_msg[] = "ENCRYPTION_TEST_PLAINTEXT_MESSAGE";
    /** Allow the plaintext to vary in size **/
    const int textSize = sizeof(plaintext_msg);

    Serial.println("-----------------------Sender-----------------------");

    Serial.println("Step 1: Define plaintext message");
    print_char(plaintext_msg, textSize - 1); /** Minus one as we don't wanna print the null terminating character **/
    Serial.println();

    /** Setup for readable text to plaintext **/
    char text[textSize];

    /** Convert char array to plaintext format; uint8_t **/
    uint8_t plaintext[textSize];
    charToUint8(plaintext_msg, plaintext, textSize);

    Serial.println("Step 2: Allocate memory for arrays: Tag, IV, plaintext & ciphertext\n");

    /** Declare arrays for Tag, IV, encryption and decryption outputs **/
    uint8_t Tag[SIZE_16];
    uint8_t IV[SIZE_16];
    uint8_t plaintextReceiver[textSize];
    uint8_t ciphertextReceiver[textSize];

    Serial.println("Step 3: Generate Random Initialization Vector");

    /** IV initialization **/
    GenerateInitializationVector(IV, SIZE_16);

    Serial.print("IV: ");
    print_uint8(IV, SIZE_16);
    Serial.println();

    Serial.println("Step 4 & 5: Perform encryption & Compute Tag (ACORN)");
    /** Encryption **/
    cipher.encryption.acorn_encryption(plaintext, ciphertextReceiver, Tag, textSize, cipher.key, IV, SIZE_16, false);

    Serial.print("Tag: ");
    print_uint8(Tag, SIZE_16);
    Serial.println();
    Serial.print("Ciphertext: ");
    print_uint8(ciphertextReceiver, 19);
    Serial.println();

    /** Manipulate packet **/
    //ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/

    /** Sender: Encapsulating IV,Tag and data into one packet **/
    int packetSize = sizeof(IV) + sizeof (Tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(IV, Tag, SIZE_16, ciphertextReceiver, packetBuffer, packetSize);

    Serial.println("Step 6: Assemble IV, Tag & Ciphertext into a single data structure");
    Serial.print("Assembled data array:");
    print_uint8(packetBuffer, packetSize);
    Serial.println();

    Serial.println("Step 7: 'Transmit' assembled data\n");
    Serial.println("----------------------Receiver----------------------");
    Serial.println("Step 1: Allocate memory for arrays: Tag, IV, plaintext & ciphertext\n");

    Serial.println("Step 2: Retrieve data as one big packet");
    Serial.print("Received data: ");
    print_uint8(packetBuffer, packetSize);
    Serial.println();

    /** Receiver: Unpack received data into IV, Tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(IV, Tag, SIZE_16, ciphertextReceiver, packetBuffer, packetSize);
    Serial.println("Step 3: Disassemble data in to IV, Tag and Ciphertext array");
    Serial.print("IV: ");
    print_uint8(IV, SIZE_16);
    Serial.print("Tag: ");
    print_uint8(Tag, SIZE_16);
    Serial.print("Ciphertext: ");
    print_uint8(ciphertextReceiver, textSize);
    Serial.println();

    /** Decryption **/
    bool decryptionValidation = cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, Tag, textSize, cipher.key, IV, SIZE_16, false);
    Serial.println("Step 4: Perform decryption of ciphertext");
    Serial.print("Decrypted plaintext: ");
    print_uint8(plaintextReceiver, textSize);
    Serial.println();

    Serial.println("Step 5: Compute Tag and Validate Tag to ensure integrity/authenticity");
    Serial.print("Validity of Tag: ");
    if (decryptionValidation){
        Serial.println("Approved");
    } else {
        Serial.println("Denied");
    }
    Serial.println();

    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    Serial.println("Step 6: Convert plaintext to human readable content");
    Serial.print("Decrypted message: ");
    print_char(text, textSize-1);
    Serial.println();
    Serial.println("Sequence finished");
}

void loop() {
}