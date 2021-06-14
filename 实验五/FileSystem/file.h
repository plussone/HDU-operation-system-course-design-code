#ifndef __FILEOPERATE_H
#define __FILEOPERATE_H

#define MAXREADER 1
#define NUM 30
#define FILE_SIZE 256
#define USERNAME_MAX_SIZE 16

typedef struct dir
{
	short num;
	char name[14];
};

int init(); //初始化文件系统
void initRootDir(); //初始化根目录
char* getPath(); //获得绝对路径
void format(); //格式化文件系统
int creatDir(char* str); //创建文件夹
void ls(); //查看当前文件夹下的详细信息
void cd(char* str); //进入文件夹
void mkdir(char* str); //创建文件夹
void rmdir(char* str); //删除文件夹
void create(char *str); //创建文件
void open(char* str); //打开文件
void close(char* str); //关闭文件
void rm(char* str); //删除文件
void exitsys(); //退出系统
void ordereader(char *str,int n); //顺序读入
void randomreader(char *str,int t,int n); //随机读入
void orderwriter(char* str); //顺序写入
void randomwriter(char* str, int s); //随机写进
void show(char* str); //显示文件详细信息
void changename(char* str,char *str2); //显示文件详细信息
void showinode(); //显示inode表
void changeblocksize();


#endif
