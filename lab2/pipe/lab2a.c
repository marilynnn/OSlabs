#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>


int main (){
	pid_t pid;
	time_t timer = time(NULL);
	int fd[2];

	pipe(fd);
	
	switch (pid = fork()){
		case -1:
			perror("Error!");
			return -1;
		case 0:
		 	printf("This is child process\n");
			printf("Reading from pipe\n");
			
			close(fd[1]); //закрываем дескриптор на запись
			read(fd[0], &pid, sizeof(pid_t));
			read(fd[0], &timer, sizeof(time_t));	
			
			sleep(5);		
			
 			printf("Parents pid = %d, parents time = %s\n",pid ,ctime(&timer));
			time(&timer);
			printf("now = %s\n",ctime(&timer));
		break;
		default:
			printf("This is parent process\n");
                        printf("Writing in pipe\n");
			close(fd[0]); //закрываем дескриптор на чтение	

			write(fd[1], &pid, sizeof(pid_t));
			write(fd[1], &timer,sizeof(time_t));
		
			close(fd[1]); //закрываем дескриптор на запись

			wait(NULL);
		break;
	}
	return 0;
}

