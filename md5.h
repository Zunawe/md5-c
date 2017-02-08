#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t* md5(uint8_t *input);

uint32_t F(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t G(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t H(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t I(uint32_t X, uint32_t Y, uint32_t Z);

void decode(uint32_t *output, unsigned char *input, size_t length);

uint32_t rotate_left(uint32_t x, uint32_t n);

void print_bytes(uint8_t *location, size_t length);
void print_hash(uint8_t *location);