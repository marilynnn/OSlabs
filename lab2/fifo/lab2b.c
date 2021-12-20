#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main (){
	pid_t pid;
	time_t timer = time(NULL);
	int fd;
	char* fifo_name = "fifo";

	if (mkfifo (fifo_name, 0777) == -1){
		perror("Failed to create FIFO");
		exit(0);
	};

	
	
	switch (pid = fork()){
		case -1:
			perror("Error!");
			exit(0);
		case 0:
		 	printf("This is child process\n");
			printf("Reading from fifo\n");
			
			fd = open(fifo_name, O_RDONLY);
			read(fd, &pid, sizeof(pid_t));
			read(fd, &timer, sizeof(time_t));	
			
			sleep(5);		
			
 			printf("Parents pid = %d, parents time = %s\n",pid ,ctime(&timer));
			time(&timer);
			printf("now = %s\n", ctime(&timer));
			close(fd);
		break;
		default:
			printf("This is parent process\n");
                        printf("Writing in fifo\n");
			fd = open(fifo_name, O_WRONLY);

			write(fd, &pid, sizeof(pid_t));
			write(fd, &timer, sizeof(time_t));
		
			close(fd);

			wait(NULL);
		break;
	}
	return 0;
}

