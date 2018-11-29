#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <error.h>
#include <errno.h>
#include <string.h>
static sem_t sem;


int main()
{
	int count = 0;
    sem_init(&sem,0,0);
	while(true)
	{
		if(sem_post(&sem) < 0) {
			printf("failed: %s\n", strerror(errno));
			break;
		}
		count++;
	}
	
	printf("threshold %d\n", count);  //2147483647
    return 0;

}