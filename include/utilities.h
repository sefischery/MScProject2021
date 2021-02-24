#ifndef MSCPROJECT2021_UTILITIES_H
#define MSCPROJECT2021_UTILITIES_H
// include guards

void print_uint8(const uint8_t *input, int size);
void print_char(char *input, int size);
void copy_uint8(const uint8_t *input, uint8_t *output, int size);
void charToUint8(const char *input, uint8_t *plaintext, int size);
void uint8ToChar(const uint8_t *plaintext, char *toText, int size);

#endif


