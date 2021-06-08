#ifndef _BLOCK_H_INCLUDED
#define _BLOCK_H_INCLUDED

#define BLOCK_MAX_NUM 1024
#define BLOCK_MAX_SIZE 1024
#define BLOCK_FILE_NAME "./block.txt"

int get_freenum_blocks(); //返回空闲盘块号
void clear_blockarray(); //清空磁盘块表
int init_block(); //初始化盘块
void freeblock0(int t); //释放直接索引盘块
void freeblock1(int t); //释放一级索引盘块
void freeblock2(int t); //释放二级索引盘块
void freeblock3(int t); //释放三级索引盘块
struct dir* getblock0d(int *pt,int t, struct dir* rdir); //获取直接索引盘块
struct dir* getblock1d(int *pt,int t, struct dir* rdir); //获取一级索引盘块
struct dir* getblock2d(int *pt,int t, struct dir* rdir); //获取二级索引盘块
struct dir* getblock3d(int *pt,int t, struct dir* rdir); //获取三级索引盘块
struct dir* writeblock0d(int* pt, int t, struct dir* rdir); //获取直接索引盘块
struct dir* writeblock1d(int* pt, int t, struct dir* rdir); //获取一级索引盘块
struct dir* writeblock2d(int* pt, int t, struct dir* rdir); //获取二级索引盘块
struct dir* writeblock3d(int* pt, int t, struct dir* rdir); //获取三级索引盘块
int writeblock(); //写入盘块到文件
char* get_block(int t); //获得磁盘块内容
void get_blocks_d(struct dir *rdir,struct inode *p,int n); //获得磁盘块内容
void write_blocks_d(struct inode *p,struct dir *pdir); //写入磁盘块内容
void clear_block_index(int t); //清楚磁盘块块号表
char* get_blockarray(int t); //获得索引块
void get_blocks_f(char* rfile, struct inode* p, int s, int n); //获得文件盘块
char* getblock0f(int* ps,int* pn, int t, char* rfile); //获取直接索引盘块
char* getblock1f(int* ps,int* pn, int t, char* rfile); //获取一级索引盘块
char* getblock2f(int* ps,int* pn, int t, char* rfile); //获取二级索引盘块
char* getblock3f(int* ps,int* pn, int t, char* rfile); //获取三级索引盘块
void write_blocks_f(char* str, struct inode* p, int s); //写入文件盘块
char* writeblock0f(int* ps, int* pn, int t, char* rfile); //获取直接索引盘块
char* writeblock1f(int* ps, int* pn, int t, char* rfile); //获取一级索引盘块
char* writeblock2f(int* ps, int* pn, int t, char* rfile); //获取二级索引盘块
char* writeblock3f(int* ps, int* pn, int t, char* rfile); //获取三级索引盘块
void chbls(int blocksize); //调整磁盘块大小

#endif

