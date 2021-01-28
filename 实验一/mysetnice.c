//测试mysetnice系统调用的程序，功能是修改nice和prio值的系统调用功能。
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>

/*340 mystenice asmlinkage long sys_mysetnice(pid_t pid,int flag,int nicevalue,void __user *prio,void __user *nice);*/

int main()
{
	pid_t pid;
	char strin[100];
	int nicevalue = 3100;
	int flag = 2;
	int p = 0;
	int n = 0;
	int *prio;
	int *nice;
	prio = &p;
	nice = &n;
	while(1)
	{
       	 	printf("输入0查看nice和prio,输入1修改nice和prio,输入其他值退出。\n");
		/*
                *     获取flag；
                *	  */
		scanf("%d",&flag);
		if(flag != 0 && flag != 1)
		{
			return 0;
		}
        	/*  
        	*     获取pid
        	*         */
        	printf("请输入pid：\n");
        	scanf("%d",&pid);
        	/*  
        	*     获取nice；
        	*         */
		if(flag == 1)
		{
			printf("请输入nice：\n");
                	scanf("%d",&nicevalue);
			if(nicevalue < -20 || nicevalue > 19)
			{
				printf("nice值非法\n");
				continue;
			}
		}
        	/*  
        	*     调用添加的系统调用；
        	*         */
        	syscall(340,pid,flag,nicevalue,prio,nice);
        	/*  
        	*     输出最新的prio以及nice；
        	*         */
        	printf("现在的nice为%d\n现在的prio为%d\n输入任意建继续\n",n,p);
		getchar();
		fgets(strin,100,stdin);
		system("clear");
	}
}
