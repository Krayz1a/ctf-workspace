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

int isDebuggerPresent() {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) return 0;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "TracerPid:", 10) == 0) {
            int pid = atoi(line + 10);
            fclose(f);
            return pid != 0;
        }
    }

    fclose(f);
    return 0;
}

typedef struct {
    int id;
    char name[16];
    int (*check_fn)(const char *);
} Agent;

int validate(const char *input) {
    const char *enc = "\xdd\xda\xf6\xd9\xc4\xc6\xf6\xce\xc7\xce\xf6\xc0\xca\xc5";
    char temp[16];
    strcpy(temp, enc);
    decrypt(temp);
    return strcmp(input, temp) == 0;
}

int useless_branch(int x) {
    if (x == 1337) return 1;
    if (x % 7 == 0) return 0;
    if (x % 42 == 0) return 0;
    return x == 1234;
}

int main() {
    if (isDebuggerPresent()) {
        printf("Nooooooo. La Policiaaa.\n");
        return 1;
    }

    Agent ag;
    ag.id = (rand() % 100) + 1; // Make sure it's never 0
    strcpy(ag.name, "root");
    ag.check_fn = &validate;

    char input[32];
    printf("What's the password?\n");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char c = (rand() % (126 - 32 + 1)) + 32;
    char c_str[2];
    c_str[0] = c;

    srand(time(NULL));
    int rand_val = rand() % 6969 + 1;
    int distract = useless_branch(rand_val);

    if (ag.check_fn(input) && ag.id != 0) {
        printf("Logged in as %s.\n", ag.name);
    } else {
        printf("Nuh Uh Nuh Uh.\n");
    }

    return 0;
}

