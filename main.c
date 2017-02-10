#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

int main(int argc, char *argv[]){
	if(argc > 1){
		for(int i = 1; i < argc; ++i){
			print_hash(md5String(argv[i]));
		}
	}
	else{
		char *input = malloc(1024);
		size_t input_size = 0;

		MD5Context ctx;
		md5Init(&ctx);

		while((input_size = fread(input, 1, 1024, stdin)) > 0){
			md5Update(&ctx, (uint8_t *)input, input_size);
		}

		md5Finalize(&ctx);
		print_hash(ctx.digest);
	}
}
