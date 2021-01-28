//sender1.c
#include "sar.h"

int main()
{
    char str[100];
    int msgid;
    struct my_msgbuf r_msg;//消息接受区
    struct my_msgbuf s_msg;

    init_signal();

    fd1=open(fifofile1,O_RDONLY);
    sem_wait(sem_over1);
    read(fd1,&msgid,sizeof(int));

    s_msg.mtype = 1;
    s_msg.sendid = 1;

    printf("tid:%u 进程(线程)1: \n", (unsigned int)pthread_self());

    while(1)
    {
        printf("发送：");
        scanf("%[^\n]%*c",str);

        sem_wait(sem_send);

        if(strcmp(str, "exit") == 0)
        {
            strcpy(s_msg.mtext, "end1");
            msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);
            sem_post(sem_receive);
            break;
        }

        strcpy(s_msg.mtext, str);
        msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);
        sem_post(sem_receive);
    }
    sem_wait(sem_over1);

    msgrcv(msgid, &r_msg, sizeof(struct my_msgbuf), 0, 0);
    printf("收到线程%u的消息: %s\n", r_msg.sendid, r_msg.mtext);

    sem_post(sem_send);
    return 0;
}
