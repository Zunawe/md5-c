#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

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

int main(int argc, char *argv[]){
	uint8_t *result;
	if(argc > 1){
		for(int i = 1; i < argc; ++i){
			result = md5String(argv[i]);
			print_hash(result);
		}
	}
	else{
		result = md5File(stdin);
		print_hash(result);
	}

	free(result);
}
