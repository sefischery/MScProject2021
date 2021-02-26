#include <cstdint>
#include <cstdio>
#include <Arduino.h>

/** Print each value of uint8 array **/
void print_uint8(const uint8_t *input, int size) {
    for(int i = 0; i < size; i++){
        printf("0x%02x ", input[i]);
        fflush(stdout);
    }
    Serial.println();
}

/** Print each value of char array **/
void print_char(char *input, int size) {
    for (int index = 0; index < size; ++index) {
        printf("%c", input[index]);
        fflush(stdout);
    }
    Serial.println();
}

/** Copy one uint8 array to another uint8 array **/
void copy_uint8(const uint8_t *input, uint8_t *output, int size) {
    for(int i = 0; i < size; i++){
        output[i] = input[i];
    }
}

/** Converts char array to uint8 array **/
void charToUint8(const char *input, uint8_t *plaintext, int size) {
    for (int index = 0; index < size; ++index) {
        plaintext[index] = input[index];
    }
}

/** Converts uint8 array to char array **/
void uint8ToChar(const uint8_t *plaintext, char *toText, int size) {
    for (int index = 0; index < size; ++index) {
        toText[index] = plaintext[index];
    }
}

void GenerateInitializationVector(uint8_t *iv, int size) {
    TransistorNoiseSource noise(A3);
    RNG.begin("RANDOM_NUMB_GEN");
    RNG.addNoiseSource(noise);

    bool haveKey = false;
    while (!haveKey)
    {
        RNG.loop();
        if(RNG.available(size)){
            RNG.rand(iv, size);
            haveKey = true;
        }
    }
}
