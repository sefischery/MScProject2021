#include <cstdint>
#include <cstdio>

void print_uint8(const uint8_t *input, int size) {
    for(int i = 0; i < size; i++){
        printf("%d", input[i]);
        fflush(stdout);
    }
}
