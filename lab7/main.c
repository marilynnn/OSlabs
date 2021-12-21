#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

pthread_rwlock_t rwlock;
int count = 0;

void* writer() {

	while(1){
		pthread_rwlock_wrlock(&rwlock);
		++count;
		pthread_rwlock_unlock(&rwlock);
		sleep(1);
	}
}

void* reader() {
		
        while(1){
                pthread_rwlock_rdlock(&rwlock);
                printf("TID: %u. Count is: %d.\n",pthread_self(),count);
                pthread_rwlock_unlock(&rwlock);
		sleep(1);
		
	}
}

int main (){
	pthread_t w, r[10];
	pthread_rwlock_init(&rwlock,NULL);

	for (int i = 0; i < 10; ++i){
		pthread_create(&r[i], NULL, reader, NULL);
	}
	pthread_create(&w, NULL, writer, NULL);

	for (int i = 0; i < 10; ++i){ 
		pthread_join(r[i],NULL);
	}
	pthread_join(w,NULL);

	pthread_rwlock_destroy(&rwlock);

	return 0;
}
