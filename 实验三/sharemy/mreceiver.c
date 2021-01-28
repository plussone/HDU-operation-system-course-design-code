//msender
#include "msar.h"

pthread_t r_thread, s_thread;

void *send(void *arg)
{
    char temp[MEM_MIN_SIZE], s_str[100];
    while(1)
    {
	printf("发送：");
	fflush(stdout);
	scanf("%[^\n]%*c",s_str);
        sem_wait(sem_sender);
        sprintf(temp,"%d:%s",*((pid_t *)arg),s_str);
        strcpy((char *)shmp, temp);
        
        sem_post(sem_receiver1);
	printf("OK\n");
        if(!strcmp(s_str,"over"))
        {
            pthread_cancel(r_thread);
            shmdt(shmp);
	    break;
        }
    }
}

void *receive(void *arg)
{
    int i;
    char r_str[100], r_str_end[100], r_str_id[10], *p;
    while(1)
    {
        sem_wait(sem_receiver2);
        strcpy(r_str, (char *)shmp);
        p = strchr(r_str,':');
        *(p++) = '\0';
        printf("\r接收到进程%s的消息：%s\n", r_str,p);
        if(strcmp(p, "over") == 0)
        {
            pthread_cancel(s_thread);
            shmdt(shmp);
            shmctl(shmid, IPC_RMID, NULL);
            sem_unlink("sender");
            sem_unlink("receiver1");
            sem_unlink("receiver2");
            break;
        }
	printf("发送：");
	fflush(stdout);
        sem_post(sem_sender);
    }
}

int main()
{
    pid_t pid = getpid();
    int res1 = 0,res2 = 0;

    init_signal();
    
    printf("进程%d启动\n",pid);
    res1 = pthread_create(&s_thread,NULL,send,&pid);
    res2 = pthread_create(&r_thread,NULL,receive,&pid);
    if(res1 || res2)
    {
        printf("create pthread failed\n");
        return 1;
    }
    pthread_join(s_thread, NULL);
    pthread_join(r_thread, NULL);
    printf("进程%d结束\n",pid);
}
