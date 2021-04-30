#include <Arduino.h>
#include <unity.h>
#include <utilities.h>
#include <Encryption_testing.h>

void test_I_equality_plaintext_output_ACORN() {
    char input[] = "ACORN_test_plaintext";
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
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(input, text, textSize);
}
void test_I_equality_plaintext_output_ASCON() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    cipher.decryption.ascon_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(input, text, textSize);
}

void test_I_equality_plaintext_output_AES_GCM() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    cipher.decryption.aes_gcm_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_EQUAL_CHAR_ARRAY(input, text, textSize);
}
void test_II_equality_tag_ACORN() {
    char input[] = "ACORN_test_plaintext";
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
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_TRUE(decryption_result);
}
void test_II_equality_tag_ASCON() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.ascon_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_TRUE(decryption_result);
}
void test_II_equality_tag_AES_GCM() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.aes_gcm_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_TRUE(decryption_result);
}

void test_III_failure_decryption_corruption_ACORN() {
    char input[] = "ACORN_test_plaintext";
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
    ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.acorn_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_FALSE(decryption_result);

}

void test_III_failure_decryption_corruption_ASCON() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.ascon_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.ascon_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_FALSE(decryption_result);

}
void test_III_failure_decryption_corruption_AES_GCM() {
    char input[] = "ACORN_test_plaintext";
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
    cipher.encryption.aes_gcm_encryption(plaintext, ciphertextReceiver, tag, textSize, cipher.key, iv, SIZE);
    ciphertextReceiver[0] = 0x03; /** This could be a MITM who is tweaking the values **/
    /** Sender: Encapsulating iv,tag and data into one packet **/
    int packetSize = sizeof(iv) + sizeof (tag) + sizeof(ciphertextReceiver);
    uint8_t packetBuffer[packetSize];
    AssembleAuthenticatedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Receiver: Unpack received data into iv, tag and informative data **/
    DisassembleAuthenticaedEncryptionPacket(iv, tag, SIZE, ciphertextReceiver, packetBuffer, packetSize);
    /** Decryption **/
    bool decryption_result = cipher.decryption.aes_gcm_decryption(ciphertextReceiver, plaintextReceiver, tag, textSize, cipher.key, iv, SIZE);
    /** Convert received plaintext to readable char array **/
    uint8ToChar(plaintextReceiver, text, textSize);
    TEST_ASSERT_FALSE(decryption_result);

}

void setup() {
    delay(2000);
    UNITY_BEGIN(); /** Start the unit test framework **/

    Serial.println("------ EXECUTING TESTS -------");
    RUN_TEST(test_I_equality_plaintext_output_ACORN);
    RUN_TEST(test_I_equality_plaintext_output_ASCON);
    RUN_TEST(test_I_equality_plaintext_output_AES_GCM);

    RUN_TEST(test_II_equality_tag_ACORN);
    RUN_TEST(test_II_equality_tag_ASCON);
    RUN_TEST(test_II_equality_tag_AES_GCM);

    RUN_TEST(test_III_failure_decryption_corruption_ACORN);
    RUN_TEST(test_III_failure_decryption_corruption_ASCON);
    RUN_TEST(test_III_failure_decryption_corruption_AES_GCM);

    UNITY_END(); /** Exit the unit test framework **/
}

void loop() {
    /** Do nothing, we execute our tests in setup() **/
}