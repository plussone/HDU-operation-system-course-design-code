//测试mychangename的系统调用，功能是改变主机名称为自定义字符串的系统调用。
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*342 mychangename smlinkage long sys_mychangename(char __user *name,int len);*/

int main()
{
	long flag = 0;
	printf("请输入修改的nodename：");
	char nodename[100];
	fgets(nodename,100,stdin);
	flag = syscall(342,nodename,strlen(nodename));
	printf("Nodename:%s\n",nodename);
}
