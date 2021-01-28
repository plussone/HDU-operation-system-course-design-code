//统计无名管道的容量程序。
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int i = 0;

int *kwrite()
{
    int pipefds[2];
    pipe(pipefds);
    char buf[1024];
    memset(buf,100,sizeof(buf));
    for (i = 0; i < 1000; ++i)
    {
        write(pipefds[1], buf, sizeof(buf));
    }   
    close(pipefds[0]);
    close(pipefds[1]);
}

int main()
{
    pthread_t thread;
    int lastnum = 0,t=0;
    pthread_create(&thread,NULL,kwrite,&i);
    while(1)
    {
        if(i != lastnum)
        {
            lastnum = i;
            t = 0;
        }
        else
        {
            t++;
            if(t == 100000)
            {
                printf("管道大小为: %dk\n", i * 1); 
                break;
            }
        }
    }
    return 0;
}
