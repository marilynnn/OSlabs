#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>

#define SHMEM_NAME "mem"

struct messege{
        time_t time;
        pid_t pid;
};

struct sembuf lock = {0, -1, 0}, unlock = {0, 1, 0};

int main (){
	struct messege rcv;
	
	key_t key = ftok(SHMEM_NAME, 1);
        if (key == -1){
                perror("ftok error");
                return -1;
        }
	int shmid = shmget(key, sizeof(struct messege), 0666);
        if (shmid == -1){
                perror("shmget error");
                return -1;
        }
	int semid = semget(key, 1,IPC_CREAT| 0666);
        if (semid == -1){
                perror("semget error");
                return -1;
        }

        struct messege* msg = shmat(shmid, NULL, 0);
        if (msg == (void*)-1){
                perror("shmat error");
                return -1;
	}
	while (1){
		semop (semid, &lock, 1);
		rcv.time = time(NULL);
		rcv.pid = getpid();
		printf("Time of receiver: %sPID of receiver: %d\n", ctime(&(rcv.time)), rcv.pid);
		printf("Sended messege: \nTime of sender: %sPID of sender: %d\n\n ", ctime(&(msg->time)), msg->pid);
		sleep(2);
		semop (semid, &unlock, 1);
	}
	shmdt(msg);
	return 0;
};
