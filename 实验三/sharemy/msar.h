//msar.h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <pthread.h>
#include <semaphore.h>//信号量头文件
#include <glob.h>

#define MEM_MIN_SIZE 1024
#define KEY 1111

sem_t *sem_sender;
sem_t *sem_receiver1;
sem_t *sem_receiver2;

key_t key;
int shmid;
void *shmp;

void init_signal()
{
    //初始化信号量
    sem_sender = sem_open("sender", O_CREAT, 0666, 1);
    sem_receiver1 = sem_open("receiver1", O_CREAT, 0666, 0);
    sem_receiver2 = sem_open("receiver2", O_CREAT, 0666, 0);

    shmid = shmget(KEY, MEM_MIN_SIZE, 0666|IPC_CREAT); //创建共享内存，key = 0(IPC_PRIVATE) 创建一个新对象。成功则返回id (一个与key相关的标识符)
    if(shmid < 0)
    {
        printf("创建共享内存出错！\n");
        exit(-1);
    }

    shmp = shmat(shmid, NULL, 0);//指定共享内存映射到新虚拟地址空间，返回起始地址
    if((long long int)shmp == -1)
    {
        printf("映射内存出错！\n");
        exit(-1);
    }
}
