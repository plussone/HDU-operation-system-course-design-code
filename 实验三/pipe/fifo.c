//实现有名管道通信程序。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>

#define fifofile "./fifomy"

int main()
{
    int fd,pipe_num;
    char buf[150];
    sem_t *Mutex;
    sem_t *send1,*send2,*send3;
    sem_t *receive1,*receive2,*receive3;
    pid_t pid1,pid2,pid3;

    memset(buf,0,sizeof(buf));
    sem_unlink("Mutex");
    sem_unlink("send1");
    sem_unlink("send2");
    sem_unlink("send3");
    sem_unlink("receive1");
    sem_unlink("receive2");
    sem_unlink("receive3");

    Mutex = sem_open("Mutex",O_CREAT,0666,1);
    send1 = sem_open("send1",O_CREAT,0666,1);
    send2 = sem_open("send2",O_CREAT,0666,1);
    send3 = sem_open("send3",O_CREAT,0666,1);
    receive1 = sem_open("receive1",O_CREAT,0666,0);
    receive2 = sem_open("receive2",O_CREAT,0666,0);
    receive3 = sem_open("receive3",O_CREAT,0666,0);

    
    mkfifo(fifofile,0666);

    pid1 = fork();
    if(pid1 > 0)
    {
        pid2 = fork();
        if(pid2 > 0)
        {
            pid3 = fork();
        }
    }

    if(pid1 < 0 || pid2 < 0 || pid3 <0)
    {
        sem_unlink(Mutex);
        sem_unlink(send1);
        sem_unlink(send2);
        sem_unlink(send3);
        sem_unlink(receive1);
        sem_unlink(receive2);
        sem_unlink(receive3);
        printf("fork error\n");
        return 2;
    }

    if(pid1 == 0)
    {
        fd=open(fifofile,O_WRONLY);
        sem_wait(send1);
        sem_wait(Mutex);
        printf("pid:%d 进程1放入数据：",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(1);
        sem_post(Mutex);
        sem_post(receive1);
    }
    else if(pid2 == 0)
    {
        fd=open(fifofile,O_WRONLY);
        sem_wait(send2);
        sem_wait(Mutex);
        printf("pid:%d 进程2放入数据：",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(1);
        sem_post(Mutex);
        sem_post(receive2);
    }
    else if(pid3 == 0)
    {
        fd=open(fifofile,O_WRONLY);
        sem_wait(send3);
        sem_wait(Mutex);
        printf("pid:%d 进程3放入数据：",getpid());
        scanf("%[^\n]%*c",buf);
        write(fd,buf,strlen(buf));
        close(fd);
        sleep(1);
        sem_post(Mutex);
        sem_post(receive3);
    }
    else
    {
	fd=open(fifofile,O_RDONLY);
        sem_wait(receive1);
        sem_wait(receive2);
        sem_wait(receive3);
        sem_wait(Mutex);
        read(fd,buf,150);
        printf("pid:%d 父进程接收数据:%s\n",getpid(),buf);
        close(fd);
        unlink(fifofile);
        sleep(1);
        sem_post(Mutex);
        sem_post(send1);
        sem_post(send2);
        sem_post(send3);
    }
}
