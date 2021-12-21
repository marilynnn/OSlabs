#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/sem.h>

#define SHMEM_NAME "mem"

struct messege{
	time_t time;	
	pid_t pid;
};

struct sembuf lock = {0, -1, 0}, unlock = {0, 1, 0};

int main (){
	
	struct messege tmp; 
	time_t msg_time;
	pid_t msg_pid;
		
	key_t key = ftok(SHMEM_NAME, 1);//получаем идентификатор
	if (key == -1){
		perror("ftok error");
		return -1;
	}

	int shmid = shmget(key, sizeof(struct messege), IPC_CREAT|0666);// создаем участок разделяемой памяти
	if (shmid == -1){
		perror("shmget error");
		return -1;
	}
	int semid = semget(key, 1, IPC_CREAT|0666);
	if (semid == -1){
		perror("semget error");
		return -1;
	}

	struct messege* msg = shmat(shmid, NULL, 0);//присоеденяемся к участку разделяемой памяти
	if (msg == (void*)-1){
		perror("shmat error");
		return -1;
	}
	struct shmid_ds buf;
	shmctl(shmid, IPC_STAT, &buf);// проверяем, чтобы передающая программа запускалась только один раз
	if (buf.shm_nattch > 1){
		printf("Already attached");
		return -1;
	}
	
	semop(semid, &unlock, 1);
	while (1){
		semop (semid, &lock, 1);
		msg_time = time(NULL);
		msg_pid = getpid();
		tmp.time = msg_time;
		tmp.pid = msg_pid;
		*msg = tmp;
		sleep(2);
		semop (semid, &unlock, 1);
	}
	semctl(semid, 0, IPC_RMID);
	shmdt(msg); // отсоединяемся от участка разделяемой памяти
	shmctl(shmid, IPC_RMID, NULL); //помечаем сегмент как удаленный
	return 0;
}
