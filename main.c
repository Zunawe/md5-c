#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char *argv[]){
	if(argc < 2){
		perror("Requires an argument string");
		exit(1);
	}

	MD5Context ctx;
	uint8_t *input = (uint8_t *)argv[1];

	md5Init(&ctx);
	md5Update(&ctx, input, strlen(argv[1]));
	md5Finalize(&ctx);
	print_hash(ctx.digest);
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