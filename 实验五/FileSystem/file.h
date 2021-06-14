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

int init(); //��ʼ���ļ�ϵͳ
void initRootDir(); //��ʼ����Ŀ¼
char* getPath(); //��þ���·��
void format(); //��ʽ���ļ�ϵͳ
int creatDir(char* str); //�����ļ���
void ls(); //�鿴��ǰ�ļ����µ���ϸ��Ϣ
void cd(char* str); //�����ļ���
void mkdir(char* str); //�����ļ���
void rmdir(char* str); //ɾ���ļ���
void create(char *str); //�����ļ�
void open(char* str); //���ļ�
void close(char* str); //�ر��ļ�
void rm(char* str); //ɾ���ļ�
void exitsys(); //�˳�ϵͳ
void ordereader(char *str,int n); //˳�����
void randomreader(char *str,int t,int n); //�������
void orderwriter(char* str); //˳��д��
void randomwriter(char* str, int s); //���д��
void show(char* str); //��ʾ�ļ���ϸ��Ϣ
void changename(char* str,char *str2); //��ʾ�ļ���ϸ��Ϣ
void showinode(); //��ʾinode��
void changeblocksize();


#endif
