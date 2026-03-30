#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc < 2){
		perror("Need argument");
		return -1;
	}

	if(!strcmp(argv[1], "CRACKME")){
		int* p = NULL;
		*p = 1;
	}

	return 0;
}
