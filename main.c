#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char *argv[]){
	uint8_t *input;
	if(argc < 2){
		input = calloc(1, 1);
	}
	else{
		input = (uint8_t *)argv[1];
	}

	uint8_t *result = md5(input);

	print_hash(result);

	// Free if we had to allocate space
	if(argc < 2){
		free(input);
	}
	free(result);
}