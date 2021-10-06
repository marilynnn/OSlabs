#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>

void exit_handler () {
	printf("Process %d, with a parent process %d, is finished. The exit_handler was triggered.\n", getpid(), getppid());
	
}

int main(){
	pid_t pid;
	int stat;
	atexit(exit_handler);

	switch (pid = fork()){
		case -1:
			perror("fork");
			exit(1);
		case 0:
			printf ("This is child process.\n");
			printf ("Process pid - %d\n", getpid());
			printf ("Parent process pid - %d\n", getppid());
			exit(0);
		default:
			printf ("This is parent process.\n");
                        printf ("Process pid - %d\n", getpid());
                        printf ("Child process pid - %d\n", pid);
			printf ("Wait while child process will call exit().\n");
			wait(&stat);
			printf ("Child process return status:%d\n", WEXITSTATUS(stat));
	}
	return 0;
} 

