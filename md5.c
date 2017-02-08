#include "md5.h"

/*
 * Constants defined by the MD5 algorithm
 */
uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;

static uint32_t s[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
				5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
				4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
				6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

static uint32_t K[] = {0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
				0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
				0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
				0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
				0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
				0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
				0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
				0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
				0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
				0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
				0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
				0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
				0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
				0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
				0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
				0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

void md5Init(MD5Context *ctx){
	ctx->size = (uint64_t)0;

	//memset(ctx->input, 0, 64);

	ctx->buffer[0] = (uint32_t)A;
	ctx->buffer[1] = (uint32_t)B;
	ctx->buffer[2] = (uint32_t)C;
	ctx->buffer[3] = (uint32_t)D;
}

void md5Update(MD5Context *ctx, uint8_t *input, size_t input_len){
	unsigned int offset = ctx->size % 64;
	ctx->size += (uint64_t)input_len;

	for(unsigned int i = 0; i < input_len; ++i){
		ctx->input[offset++] = (uint8_t)*(input + i);

		if(offset % 64 == 0){
			for(unsigned int j = 0; j < 4; ++j){
				ctx->input[j] = reverse_bytes((uint32_t)input[j * 4]);
			}
			md5Step(ctx->buffer, ctx->input);
			offset = 0;
		}
	}
}

void md5Finalize(MD5Context *ctx){

}

void md5Step(uint32_t *buffer, uint8_t *input){
	uint32_t A = buffer[0];
	uint32_t B = buffer[1];
	uint32_t C = buffer[2];
	uint32_t D = buffer[3];

	uint32_t E;

	unsigned int j;

	for(unsigned int i = 0; i < 64; ++i){
		switch(i / 16){
			case 0:
				E = F(B, C, D);
				j = i;
				break;
			case 1:
				E = G(B, C, D);
				j = ((i * 5) + 1) % 16;
				break;
			case 2:
				E = H(B, C, D);
				j = ((i * 3) + 5) % 16;
				break;
			default:
				E = I(B, C, D);
				j = (i * 7) % 16;
				break;
		}

		uint32_t temp = D;
		D = C;
		C = B;
		B = B + rotate_left(A + E + K[i] + input[j], s[i]);
		A = temp;
	}

	buffer[0] += A;
	buffer[1] += B;
	buffer[2] += C;
	buffer[3] += D;
}

/*
 * Uses MD5 algorithm to hash a string.
 * Returns a pointer to 128 bits of the hash
 */
uint8_t* md5(uint8_t *input){
	uint64_t input_len = strlen((char *)input);
	uint64_t extended_len_bits;

	// Set extended_len_bits to the input length and then increase it until
	// extended_len_bits mod 512 == 448
	for(extended_len_bits = 0; extended_len_bits % 512 != 448; ++extended_len_bits);

	// Allocate memory for the message plus an extra 64 bits for the length
	// calloc zeroes the memory
	uint8_t *message = calloc((extended_len_bits + 64) / 8, 1);
	uint64_t buffer_size_bits = extended_len_bits + 64;

	// Copy the input to the beginning of the buffer and put a 1 bit right after
	memcpy(message, input, input_len);
	message[input_len] = 0x80;

	// Calculate the number of bits in the input and put that in the last
	// 64 bits of the buffer
	uint32_t upper_half = (input_len * 8) >> 32;
	uint32_t lower_half = (input_len * 8);
	memcpy(message + (extended_len_bits / 8), &lower_half, 4);
	memcpy(message + ((extended_len_bits + 32) / 8), &upper_half, 4);

	// Loop over blocks of 16 32-bit words
	for(unsigned int i = 0; i < (buffer_size_bits / 32) / 16; ++i){
		unsigned char *block = (unsigned char *)(message + (i * 16));
		uint32_t X[16];
		uint32_t J, L;
		uint32_t AA = A;
		uint32_t BB = B;
		uint32_t CC = C;
		uint32_t DD = D;

		// Write string of chars to little-endian
		decode(X, block, 64);

		// Shortened version of the list of steps to perform on a block
		for(unsigned int j = 0; j < 64; ++j){
			switch(j / 16){
				case 0:
					J = F(B, C, D);
					L = j;
					break;
				case 1:
					J = G(B, C, D);
					L = ((j * 5) + 1) % 16;
					break;
				case 2:
					J = H(B, C, D);
					L = ((j * 3) + 5) % 16;
					break;
				default:
					J = I(B, C, D);
					L = (j * 7) % 16;
					break;
			}

			uint32_t temp = D;
			D = C;
			C = B;
			B = B + rotate_left(A + J + K[j] + X[L], s[j]);
			A = temp;
		}

		// Add the original values back in to accumulate
		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}

	// Allocate the pointer to return and copy the digest in
	uint8_t *digest = malloc(16);
	memcpy(digest, &A, 4);
	memcpy(digest + 4, &B, 4);
	memcpy(digest + 8, &C, 4);
	memcpy(digest + 12, &D, 4);

	// Clean up memory
	free(message);

	return digest;
}

/*
 * Bit-manipulation functions defined by the MD5 algorithm
 */
uint32_t F(uint32_t X, uint32_t Y, uint32_t Z){
	return (X & Y) | (~X & Z);
}

uint32_t G(uint32_t X, uint32_t Y, uint32_t Z){
	return (X & Z) | (Y & ~Z);
}

uint32_t H(uint32_t X, uint32_t Y, uint32_t Z){
	return X ^ Y ^ Z;
}

uint32_t I(uint32_t X, uint32_t Y, uint32_t Z){
	return Y ^ (X | ~Z);
}

/*
 * Rotates a 32-bit word left by n bits
 */
uint32_t rotate_left(uint32_t x, uint32_t n){
	return (x << n) | (x >> (32 - n));
}

/*
 * Reorders the bytes
 */
uint32_t reverse_bytes(uint32_t word){
	return ((word & 0xFF000000) >> 24) |
	       ((word & 0x00FF0000) >>  8) |
	       ((word & 0x0000FF00) <<  8) |
	       ((word & 0x000000FF) << 24);
}

void decode(uint32_t *output, unsigned char *input, size_t length){
	for(unsigned int i = 0, j = 0; j < length; ++i, j += 4){
		output[i] = ((uint32_t)input[j] | ((uint32_t)input[j + 1] << 8) | ((uint32_t)input[j + 2] << 16) | ((uint32_t)input[j + 3] << 24));
	}
}

/*
 * Printing bytes from buffers or the hash
 */
void print_bytes(void *p, size_t length){
	uint8_t *pp = (uint8_t *)p;
	for(unsigned int i = 0; i < length; ++i){
		if(i && !(i % 16)){
			printf("\n");
		}
		printf("%02X ", pp[i]);
	}
	printf("\n");
}

void print_hash(uint8_t *p){
	for(unsigned int i = 0; i < 16; ++i){
		printf("%02x", p[i]);
	}
	printf("\n");
}
