#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cv;

int count = 0;

void* writer() {

	while(1){
		pthread_mutex_lock(&mutex);
		++count;
		pthread_cond_broadcast(&cv);
		pthread_mutex_unlock(&mutex);
		sleep(2);
	}
}

void* reader() {
		
        while(1){
                pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cv,&mutex);
                printf("TID: %u. Count is: %d.\n",pthread_self(),count);
                pthread_mutex_unlock(&mutex);
		
	}
}

int main (){
	pthread_t w, r[10];
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cv,NULL);
	
	for (int i = 0; i < 10; ++i){
		pthread_create(&r[i], NULL, reader, NULL);
	}
	pthread_create(&w, NULL, writer, NULL);

	pthread_join(w,NULL); 

	for (int i = 0; i < 10; ++i){ 
		pthread_join(r[i],NULL);
	}

	pthread_cond_destroy(&cv);
	pthread_mutex_destroy(&mutex);

	return 0;
}
