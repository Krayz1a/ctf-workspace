#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define KEY 0xA9

void decrypt(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] ^= KEY;
    }
}  

int main() {
	const char *enc = "\xdd\xda\xf6\xd9\xc4\xc6\xf6\xce\xc7\xce\xf6\xc0\xca\xc5";
    char temp[16];
    strcpy(temp, enc);
    decrypt(temp);
    printf("%s\n", temp);

    return 0;
}

