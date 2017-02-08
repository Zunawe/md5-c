#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

int main(void){
	MD5Context ctx;

	md5Init(&ctx);
	printf("%lu\n", ctx.size);
	print_bytes(ctx.input, 64);
	printf("\n");
	print_bytes(ctx.buffer, 16);
	printf("\n");

	uint8_t *input = malloc(70);
	memset(input, 0xAB, 35);
	memset(input + 35, 0xCD, 35);
	md5Update(&ctx, input, 70);
	printf("%lu\n", ctx.size);
	print_bytes(ctx.input, 64);
	printf("\n");
	print_bytes(ctx.buffer, 16);
	printf("\n");
}

/*int main(int argc, char *argv[]){
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
}*/

/*int main(void){
	char *input = NULL;
	size_t input_size = 0;
	ssize_t read = 0;

	if((read = getdelim(&input, &input_size, -1, stdin)) == -1){
		free(input);
		perror("Error reading from stdin");
		exit(1);
	}

	if(ferror(stdin)){
		free(input);
		perror("Error reading from stdin");
		exit(2);
	}
	//input[read - 1] = 0;

	uint8_t *result = md5((uint8_t *)input);

	print_hash(result);
	free(input);
	free(result);
}*/