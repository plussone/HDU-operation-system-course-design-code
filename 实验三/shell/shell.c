//实现一个模拟的shell，基本功能加find、grep命令。
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

void clear_space(char *str,char *cmd)
{
    int i = 0, j = 0;
    while(str[i] == ' ')
    {
        i++;
    }
    while(str[i] != ' ')
    {
        cmd[j++] = str[i++];
    }
    cmd[j] = '\0';
}

int main()
{
    char command[100],cmd[55],token[50];
    int status;
    pid_t pid;
    while(1)
    {
        printf("shell:");
        scanf("%[^\n]%*c",command);
        status = -1;
        clear_space(command,token);
        if(!strcmp(token,"cmd1") || !strcmp(token,"cmd2") || !strcmp(token,"cmd3"))
        {
            pid = fork();
            if(pid < 0)
            {
                printf("fork error\n");
            }
            else if(pid > 0)
            {
                wait(&status);    
            }
            else
            {
                sprintf(cmd,"./%s",token);
                system(cmd);
		exit(0);
            }
        }
        else if(!strcmp(token,"exit"))
        {
            printf("shell exit\n");
	    break ;
        }
        else if(!strcmp(token,"find") || !strcmp(token,"grep"))
        {
            system(command);
        }
        else
        {
            printf("Command not found\n");
        }
    }
}
