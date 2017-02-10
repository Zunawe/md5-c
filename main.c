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
		print_hash(md5File(stdin));
	}
}
