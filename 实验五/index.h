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

//inode�ṹ��
typedef struct inode
{
	int mode;        //�ļ�����Ȩ������
	char type[TYPE_MAX_SIZE];      //�ļ�����
	char username[USERNAME_MAX_SIZE];  //�ļ�ӵ���߱�ʶ
	short blocknum[INDEX_NUM]; //�ļ�������ʼ�̿��
	int fileSize;       //�ļ���С���̿�Ϊ��λ
	int dataSize;       //��д������ݴ�С���ֽ�Ϊ��λ
	time_t visittime;   //�ļ����һ�η���ʱ��
	time_t createtime;      //�ļ�����ʱ��
	time_t changetime;     //�ļ����һ���޸�ʱ��
};

typedef struct tdinode
{
	int t;
	int number;
	char* str;
	struct dir* fdir;
	struct tdinode* next; //��һ���ڵ�
};

//dinode
typedef struct dinode
{
	int number; //�����ڵ�
	char fileName[PATH_MAX_SIZE]; //�ļ���
	struct dinode* next; //��һ���ڵ�
};//inode�ڵ�����

void clear_inodearray(); //���������
short get_freenum_inodearray(); //��ÿ���������
int get_filenum_inodearray(char *filename); //����ļ�������
int init_index(); //��ʼ��������
void move_filenum_inodearry(char *filename); //ɾ������
void freeindex(int t); //ɾ������
int writeindex(); //д���������ļ�
struct dinode* get_phdinode(); //�������ͷָ��
struct inode* get_inodearray(int t); //��������ڵ�
void init_phdinode(); //��ʼ������ָ��
int check_filename(char* str,char* type); //����ļ����Ƿ��ظ�
struct dir *read_dir(struct inode* p); //����ļ�����Ŀ¼
void write_dir(struct inode *p,struct dir *pdir); //д���ļ�����Ŀ¼
char* oread_file(struct inode *p,int n); //˳������ļ�
char* rread_file(struct inode *p,int ps,int pn); //��������ļ�
void write_file(struct inode* p, char* str, int ps); //д���ļ�
void showinodearray(); //��ʾ�ļ�������
void changeblocksize(int blocksize); //�������̿��С

#endif
