#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "md5.h"

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
	}

	free(result);
}
