#include<stdio.h>
#include<stdlib.h>
#include<sys/sem.h>
#include <error.h>
#include <errno.h>
#include <string.h>
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
int sem_id;
int set_semvalue()
{
    union semun sem_union;    
    sem_union.val = 1;
    if(semctl(sem_id,0,SETVAL,sem_union)==-1)
        return 0;
    return 1;
}
int semaphore_p()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;
    //sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id,&sem_b,1)==-1)
    {
        fprintf(stderr,"semaphore_p failed\n");
        return 0;
    }
    return 1;
}
int semaphore_v()
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;
    //sem_b.sem_flg = SEM_UNDO;
	printf("v");
    if(semop(sem_id,&sem_b,1)==-1)
    {
        fprintf(stderr,"semaphore_v failed %s\n", strerror(errno));
        return 0;
    }
    return 1;
}
void del_semvalue()
{
    //删除信号量
    union semun sem_union;
    if(semctl(sem_id,0,IPC_RMID,sem_union)==-1)
        fprintf(stderr,"Failed to delete semaphore\n");
}
int main(int argc,char *argv[])
{
    char message = 'x';
    //创建信号量
     sem_id = semget((key_t)1234,1,0666|IPC_CREAT);
    if(argc>1)
    {
        //初始化信号量
        if(!set_semvalue())
        {
            fprintf(stderr,"init failed\n");
            exit(EXIT_FAILURE);
        }
        //参数的第一个字符赋给message
        message = argv[1][0];
    }
    int i=0;
    for(i=0;i<5000;i++)
    {
		#if 0
        //等待信号量
        if(!semaphore_p())
            exit(EXIT_FAILURE);
        printf("%c",message);
        fflush(stdout);
        //sleep(1);
		#else
        //发送信号量
        if(!semaphore_v())
            exit(EXIT_FAILURE);
        //sleep(1);
		#endif
    }
    //printf("\n%d-finished\n",getpid());
    if(argc>1)
    {
        //退出前删除信号量
        //del_semvalue();
    }
    exit(EXIT_SUCCESS);
}