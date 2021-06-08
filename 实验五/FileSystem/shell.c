#pragma warning(disable:4996)

#include "index.h"
#include "file.h"
#include "block.h"

/*清除空格*/
void clear_space(char* str, char* cmd)
{
	int i = 0, j = strlen(str) - 1;
	while (str[i] == ' ')
	{
		i++;
	}
	while (str[j] == ' ')
	{
		j--;
	}
	strncpy(cmd, str + i, j - i + 1);
	cmd[j - i + 1] = '\0';
}

/* 获得当前目录名 */
void GetDir()
{
	char* path = getPath();
	printf("%s>", path);
	fflush(stdout);
}

int Separate(char* str, char(*astr)[100], const char* c)
{
	int n = 0;
	char* s;
	/* 获取第一个子字符串 */
	s = strtok(str, c);

	/* 继续获取其他的子字符串 */
	while (s != NULL)
	{
		strcpy(astr[n++], s);
		s = strtok(NULL, c);
	}
	return n;
}

int pin(char(*args)[100])
{
	const char c[2] = " ";
	char command[1024] = "", token[1024];
	scanf("%[^\n]%*c", command);
	while (!strlen(command))
	{
		GetDir();
		rewind(stdin);
		scanf("%[^\n]%*c", command);
	}
	clear_space(command, token);
	return Separate(token, args, c);
}

int strtonum(char* str)
{
	int t = strlen(str),sum = 0,fh = 1,i = 0;
	if (str[i] == '-')
	{
		fh = -1;
		i++;
	}
	for(;i < t;i++)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return -1;
		}
		sum *= 10;
		sum += str[i] - '0';
	}
	return sum * fh;
}

int back_path(char *str1)
{
	const char c[2] = "/";
	char token[100][100];
	int m = Separate(str1, token, c);
	char npath[PATH_MAX_SIZE];
	char mpath[PATH_MAX_SIZE];
	if (m < 1)
	{
		return -1;
	}
	strcpy(npath,token[0]);
	strcpy(mpath, token[0]);
	for (int i = 1; i < m - 1;i++)
	{
		sprintf(mpath, "%s/%s", npath,token[i]);
		strcpy(npath, mpath);
	}
	strcpy(str1, mpath);
	return 0;
}

int main()
{
	init();
	printf("/*******欢迎使用文件系统******/\n");
	while (1)
	{
		GetDir();
		//读取字符串
		char args[100][100];
		int n = pin(args);
		if (!strcmp(args[0], "format"))
		{
			if (n == 1)
			{
				printf("确定要格式化磁盘吗?(Y/N)");
				char s;
				scanf("%c", &s);
				rewind(stdin);
				if (s == 'Y' || s == 'y')
				{
					format();
					printf("格式化成功\n");
				}
				else
				{
					printf("取消格式化\n");
				}
			}
			else
			{
				printf("命令输入格式错误[format:format]\n");
			}
		}
		else if (!strcmp(args[0], "mkdir"))
		{
			for (int i = 1; i < n; i++)
			{
				int t = strlen(args[i]);
				if (t < FILENAME_MAX_SIZE)
				{
					mkdir(args[i]);
				}
				else
				{
					printf("error:文件名不能超过14字节，%s(%d字节)\n", args[i], t);
				}
			}
		}
		else if (!strcmp(args[0], "rmdir"))
		{
			if (n != 1)
			{
				for (int i = 1; i < n; i++)
				{
					rmdir(args[1]);
				}
			}
			else
			{
				printf("命令输入格式错误[rmdir:rmdir filename]\n");
			}
		}
		else if (!strcmp(args[0], "ls"))
		{
			if (n == 1)
			{
				ls();
			}
			else
			{
				printf("命令输入格式错误[ls:ls]\n");
			}
		}
		else if (!strcmp(args[0], "cd"))
		{
			if (n == 2)
			{
				const char c[2] = "/";
				char token[100][100];
				int m = Separate(args[1], token, c);
				char npath[PATH_MAX_SIZE];
				strcpy(npath, getPath());
				char mpath[PATH_MAX_SIZE] = "";
				if (m == 1 && strcmp(token[0], "..") && strcmp(token[0], "~") && strcmp(token[0], "."))
				{
					sprintf(mpath,"%s/%s",npath, token[0]);
				}
				else if(!strcmp(token[0],"~"))
				{
					strcpy(npath, token[0]);
					strcpy(mpath, token[0]);
					for (int i = 1; i < m; i++)
					{
						sprintf(mpath, "%s/%s", npath, token[i]);
						strcpy(npath, mpath);
					}
				}
				else if (!strcmp(token[0], ".."))
				{
					int i = 0;
					while (!strcmp(token[i], ".."))
					{
						if (!back_path(npath) == 0)
						{
							printf("输入地址错误\n");
							break;
						}
						i++;
					}
					strcpy(mpath, npath);
					for (; i < m; i++)
					{
						sprintf(mpath, "%s/%s", npath, token[i]);
						strcpy(npath, mpath);
					}
				}
				else if (!strcmp(token[0], "."))
				{
					strcpy(mpath, npath);
					for (int i = 1; i < m; i++)
					{
						sprintf(mpath, "%s/%s", npath, token[i]);
						strcpy(npath, mpath);
					}
				}
				else
				{
					printf("输入地址错误\n");
				}
				cd(mpath);
			}
			else
			{
				printf("命令输入格式错误[cd:cd path]\n");
			}
		}
		else if (!strcmp(args[0], "create"))
		{
			if (n > 1)
			{
				for (int i = 1; i < n; i++)
				{
					create(args[1]);
				}
			}
			else
			{
				printf("命令输入格式错误[create:create filename]\n");
			}
		}
		else if (!strcmp(args[0], "open"))
		{
			//open(args[1]);
		}
		else if (!strcmp(args[0], "close"))
		{
			//close(args[1]);
		}
		else if (!strcmp(args[0], "write"))
		{
			if (n == 2)
			{
				orderwriter(args[1]);
			}
			else if (n == 3)
			{
				int t = strtonum(args[2]);
				randomwriter(args[1], t);
			}
			else
			{
				printf("命令输入格式错误[write:write filename | start]\n");
			}
		}
		else if (!strcmp(args[0], "read"))
		{
			if (n == 3)
			{
				int np = strtonum(args[2]);
				ordereader(args[1],np);
			}
			else if(n == 4)
			{
				int t = strtonum(args[2]);
				int np = strtonum(args[3]);
				randomreader(args[1], t,np);
			}
			else
			{
				printf("命令输入格式错误[read:read filename | start num]\n");
			}
		}
		else if (!strcmp(args[0], "rm"))
		{
			if (n != 1)
			{
				for (int i = 1; i < n; i++)
				{
					rm(args[i]);
				}
			}
			else
			{
				printf("命令输入格式错误[rm:rm filename]\n");
			}
		}
		else if (!strcmp(args[0], "exitsys"))
		{
			if (n == 1)
			{
				exitsys();
				break;
			}
			else
			{
				printf("命令输入格式错误[exitsys:exitsys]\n");
			}
		}
		else if (!strcmp(args[0], "changename"))
		{
			changename(args[1],args[2]);
		}
		else if (!strcmp(args[0], "show"))
		{
			show(args[1]);
		}
		else if (!strcmp(args[0], "showinode"))
		{
			showinode();
		}
		else if (!strcmp(args[0], "changeblocksize"))
		{
			int np = strtonum(args[1]);
			changeblocksize(np);
		}
		else
		{
			printf("command not found\n");
		}
	}
	return 0;
}
