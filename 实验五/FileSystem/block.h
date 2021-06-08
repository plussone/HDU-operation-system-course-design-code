#ifndef _BLOCK_H_INCLUDED
#define _BLOCK_H_INCLUDED

#define BLOCK_MAX_NUM 1024
#define BLOCK_MAX_SIZE 1024
#define BLOCK_FILE_NAME "./block.txt"

int get_freenum_blocks(); //���ؿ����̿��
void clear_blockarray(); //��մ��̿��
int init_block(); //��ʼ���̿�
void freeblock0(int t); //�ͷ�ֱ�������̿�
void freeblock1(int t); //�ͷ�һ�������̿�
void freeblock2(int t); //�ͷŶ��������̿�
void freeblock3(int t); //�ͷ����������̿�
struct dir* getblock0d(int *pt,int t, struct dir* rdir); //��ȡֱ�������̿�
struct dir* getblock1d(int *pt,int t, struct dir* rdir); //��ȡһ�������̿�
struct dir* getblock2d(int *pt,int t, struct dir* rdir); //��ȡ���������̿�
struct dir* getblock3d(int *pt,int t, struct dir* rdir); //��ȡ���������̿�
struct dir* writeblock0d(int* pt, int t, struct dir* rdir); //��ȡֱ�������̿�
struct dir* writeblock1d(int* pt, int t, struct dir* rdir); //��ȡһ�������̿�
struct dir* writeblock2d(int* pt, int t, struct dir* rdir); //��ȡ���������̿�
struct dir* writeblock3d(int* pt, int t, struct dir* rdir); //��ȡ���������̿�
int writeblock(); //д���̿鵽�ļ�
char* get_block(int t); //��ô��̿�����
void get_blocks_d(struct dir *rdir,struct inode *p,int n); //��ô��̿�����
void write_blocks_d(struct inode *p,struct dir *pdir); //д����̿�����
void clear_block_index(int t); //������̿��ű�
char* get_blockarray(int t); //���������
void get_blocks_f(char* rfile, struct inode* p, int s, int n); //����ļ��̿�
char* getblock0f(int* ps,int* pn, int t, char* rfile); //��ȡֱ�������̿�
char* getblock1f(int* ps,int* pn, int t, char* rfile); //��ȡһ�������̿�
char* getblock2f(int* ps,int* pn, int t, char* rfile); //��ȡ���������̿�
char* getblock3f(int* ps,int* pn, int t, char* rfile); //��ȡ���������̿�
void write_blocks_f(char* str, struct inode* p, int s); //д���ļ��̿�
char* writeblock0f(int* ps, int* pn, int t, char* rfile); //��ȡֱ�������̿�
char* writeblock1f(int* ps, int* pn, int t, char* rfile); //��ȡһ�������̿�
char* writeblock2f(int* ps, int* pn, int t, char* rfile); //��ȡ���������̿�
char* writeblock3f(int* ps, int* pn, int t, char* rfile); //��ȡ���������̿�
void chbls(int blocksize); //�������̿��С

#endif

