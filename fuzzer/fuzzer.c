#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("Need Argument");
		return -1;
	}

	pid_t p = fork();
	if(p<0){
		perror("Error forking"); return -1;
	}
	else if(p==0){
		char path[256];
		snprintf(path, sizeof(path), "./%s", argv[1]);
		char *target_args[] = {path, "CRACKME", NULL};
		execvp(path, target_args);

		exit(1);
	} else {
		int status;
		waitpid(p, &status, 0);
		
		if(WIFSIGNALED
	}


	return 0;
}
