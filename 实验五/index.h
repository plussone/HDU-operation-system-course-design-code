#ifndef __INDEXOPERATE_H
#define __INDEXOPERATE_H

#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define FILENAME_MAX_SIZE 14
#define PATH_MAX_SIZE 128
#define USERNAME_MAX_SIZE 16
#define TYPE_MAX_SIZE 25
#define INDEX_NUM 13
#define DIRTABLE_MAX_SIZE 800
#define INDEX_FILE_NAME "./index.txt"
#define YULIUXIANG_NUM 100
#define YULIUXIANG_FILE_NUM 2048
#define YULIUXIANG_WRITE_NUM 1024 * 1024

//inode结构：
typedef struct inode
{
	int mode;        //文件访问权限属性
	char type[TYPE_MAX_SIZE];      //文件类型
	char username[USERNAME_MAX_SIZE];  //文件拥有者标识
	short blocknum[INDEX_NUM]; //文件数据起始盘块号
	int fileSize;       //文件大小，盘块为单位
	int dataSize;       //已写入的内容大小，字节为单位
	time_t visittime;   //文件最后一次访问时间
	time_t createtime;      //文件创建时间
	time_t changetime;     //文件最后一次修改时间
};

typedef struct tdinode
{
	int t;
	int number;
	char* str;
	struct dir* fdir;
	struct tdinode* next; //下一个节点
};

//dinode
typedef struct dinode
{
	int number; //索引节点
	char fileName[PATH_MAX_SIZE]; //文件名
	struct dinode* next; //下一个节点
};//inode节点数组

void clear_inodearray(); //清空索引表
short get_freenum_inodearray(); //获得空闲索引号
int get_filenum_inodearray(char *filename); //获得文件索引号
int init_index(); //初始化索引表
void move_filenum_inodearry(char *filename); //删除索引
void freeindex(int t); //删除索引
int writeindex(); //写入索引到文件
struct dinode* get_phdinode(); //获得链表头指针
struct inode* get_inodearray(int t); //获得索引节点
void init_phdinode(); //初始化链表指针
int check_filename(char* str,char* type); //检查文件名是否重复
struct dir *read_dir(struct inode* p); //获得文件夹子目录
void write_dir(struct inode *p,struct dir *pdir); //写入文件夹子目录
char* oread_file(struct inode *p,int n); //顺序读入文件
char* rread_file(struct inode *p,int ps,int pn); //随机读入文件
void write_file(struct inode* p, char* str, int ps); //写入文件
void showinodearray(); //显示文件索引表
void changeblocksize(int blocksize); //调整磁盘块大小

#endif
