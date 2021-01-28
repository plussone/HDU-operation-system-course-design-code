//线程间的通信sar.h
//IPC消息队列
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <pthread.h>
#include <semaphore.h>//信号量头文件

#define fifofile1 "/tmp/fifomy1"
#define fifofile2 "/tmp/fifomy2"

int fd1,fd2;//消息队列标识符
//struct my_msgbuf r_msg;//消息接受区
//struct my_msgbuf s_msg;

sem_t *sem_send;
sem_t *sem_receive;
sem_t *sem_over1;
sem_t *sem_over2;
sem_t *sem_receive1;
sem_t *sem_receive2;

//缓冲区mymsg
struct my_msgbuf
{
    long mtype;//消息类型1为发送者的消息，2为接收的消息。
    int sendid;//1为发送者1，2为发送者2。
    char mtext[100];
};

void init_signal()
{
    //初始化信号量
    sem_send = sem_open("send", O_CREAT, 0666, 1);
    sem_receive = sem_open("receive", O_CREAT, 0666, 0);
    sem_over1 = sem_open("over1", O_CREAT, 0666, 0);
    sem_over2 = sem_open("over2", O_CREAT, 0666, 0);
}
