//receiver.c
#include "sar.h"

int main()
{
    int msgid;
    int flag_over1 = 0;
    int flag_over2 = 0;
    struct my_msgbuf r_msg;//消息接受区
    struct my_msgbuf s_msg;

    msgid = msgget(IPC_PRIVATE, 0666|IPC_CREAT);

    sem_unlink("send");
    sem_unlink("receive");
    sem_unlink("over1");
    sem_unlink("over2");

    init_signal();

    mkfifo(fifofile1,0666);
    mkfifo(fifofile2,0666);

    fd1=open(fifofile1,O_WRONLY);
    fd2=open(fifofile2,O_WRONLY);
    write(fd1,&msgid,sizeof(int));
    sem_post(sem_over1);
    write(fd2,&msgid,sizeof(int));
    sem_post(sem_over2);

    //id = (unsigned int)pthread_self();
    s_msg.mtype = 2;
    s_msg.sendid = 3;

    printf("tid:%u 进程(线程)3：\n", (unsigned int)pthread_self());

    while (1)
    {
        sem_wait(sem_receive);
        msgrcv(msgid, &r_msg, sizeof(struct my_msgbuf), 0, 0);
        printf("收到线程%u的消息: %s\n", r_msg.sendid, r_msg.mtext);

        if (r_msg.sendid == 1)
        {
            if (strcmp(r_msg.mtext, "end1") == 0)
            {
                printf("发送给线程1：over1\n");
                strcpy(s_msg.mtext, "over1");
                msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

                sem_post(sem_over1);
                flag_over1 = 1;
            }
            else
            {
                sem_post(sem_send);
            }
        }
        else if(r_msg.sendid == 2)
        {
            if (strcmp(r_msg.mtext, "end2") == 0)
            {
                printf("发送给线程2：over2\n");\
                strcpy(s_msg.mtext, "over2");
                msgsnd(msgid, &s_msg, sizeof(struct my_msgbuf), 0);

                sem_post(sem_over2);
                flag_over2 = 1;
            }
            else
            {
                sem_post(sem_send);
            }
        }

        if (flag_over1 && flag_over2)
            break;
    }


    sem_unlink("send");
    sem_unlink("receive");
    sem_unlink("over1");
    sem_unlink("over2");

    return 0;
}
