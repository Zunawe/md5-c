#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint8_t* md5(uint8_t *input);

uint32_t F(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t G(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t H(uint32_t X, uint32_t Y, uint32_t Z);
uint32_t I(uint32_t X, uint32_t Y, uint32_t Z);

void decode(uint32_t *output, unsigned char *input, size_t length);

uint32_t rotate_left(uint32_t x, uint32_t n);

void print_bytes(void *p, size_t length);
void print_hash(uint8_t *p);

typedef struct{
	uint64_t size;
	uint32_t buffer[4];
	uint8_t input[64];
	uint8_t digest[16];
}MD5Context;

void md5Init(MD5Context *ctx);
void md5Update(MD5Context *ctx, uint8_t *input, size_t input_len);
void md5Finalize(MD5Context *ctx);
void md5Step(uint32_t *buffer, uint32_t *input);

uint32_t reverse_bytes(uint32_t word);
