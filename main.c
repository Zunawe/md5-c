#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

/*int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Usage: ./md5 [STRING] ...\n");
		return 0;
	}

	for(int i = 1; i < argc; ++i){
		MD5Context ctx;
		uint8_t *input = (uint8_t *)argv[i];

		md5Init(&ctx);
		md5Update(&ctx, input, strlen(argv[i]));
		md5Finalize(&ctx);
		print_hash(ctx.digest);
	}
}*/

int main(void){
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
		exit(1);
	}

	MD5Context ctx;
	md5Init(&ctx);

	md5Update(&ctx, (uint8_t *)input, read);
	md5Finalize(&ctx);
	print_hash(ctx.digest);
}
