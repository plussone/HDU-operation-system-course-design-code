#pragma warning(disable:4996)

#include "index.h"
#include "block.h"
#include "file.h"

struct inode inodearray[DIRTABLE_MAX_SIZE]; //索引块
struct dinode* phdinode; //索引表头节点

void clear_inodearray()
{
	phdinode->number = 0;
	struct dinode* pn;
	pn = phdinode->next;
	phdinode->next = NULL;
	while (pn != NULL)
	{
		struct dinode* t = pn->next;
		free(pn);
		pn = t;
	}
	for (int i = 0; i < DIRTABLE_MAX_SIZE; i++)
	{
		inodearray[i].mode = -1;
	}
}

short get_freenum_inodearray()
{
	for (int i = 0; i < DIRTABLE_MAX_SIZE; i++)
	{
		if (inodearray[i].mode == -1)
		{
			return i;
		}
	}
	return -1;
}

int get_filenum_inodearray(char* filename)
{
	struct dinode* pt;
	pt = phdinode->next;
	while (pt != NULL)
	{
		if (!strcmp(pt->fileName, filename))
		{
			return pt->number;
		}
		pt = pt->next;
	}
	return -1;
}

int init_index()
{
	FILE* fp;
	if ((fp = fopen(INDEX_FILE_NAME, "r")) == NULL)
	{
		return -1;
	}
	int sum;
	fscanf(fp, "%d", &sum);
	struct dinode* pnow = phdinode;
	phdinode->number = sum;
	//循环读取文件的每一行数据
	while (sum--)
	{
		struct dinode* pt = (struct dinode*)malloc(sizeof(struct dinode));
		fscanf(fp, "%d%s", &(pt->number), pt->fileName);
		pnow->next = pt;
		pnow = pt;
	}
	pnow->next = NULL;
	for (int i = 0; i < DIRTABLE_MAX_SIZE; i++)
	{
		fscanf(fp, "%d%s%s%d%d%lld%lld%lld", &inodearray[i].mode, inodearray[i].type, inodearray[i].username, &inodearray[i].fileSize, &inodearray[i].dataSize, &inodearray[i].visittime, &inodearray[i].changetime, &inodearray[i].createtime);
		for (int j = 0; j < INDEX_NUM; j++)
		{
			fscanf(fp, "%hd", &(inodearray[i].blocknum[j]));
		}
	}
	fclose(fp);
	return 0;
}

void move_filenum_inodearry(char* filename)
{
	struct dinode* pt, * p;
	p = phdinode;
	pt = phdinode->next;
	while (pt != NULL)
	{
		if (!strcmp(pt->fileName, filename))
		{
			inodearray[pt->number].mode = -1;
			p->next = pt->next;
			free(pt);
			return;
		}
		p = pt;
		pt = pt->next;
	}
}

void freeindex(int t)
{
	inodearray[t].mode = -1;
	for (int i = 0; i < INDEX_NUM - 3; i++)
	{
		if (inodearray[t].blocknum[i] > 0)
		{
			freeblock0(inodearray[t].blocknum[i]);
		}
	}
	if (inodearray[t].blocknum[10] > 0)
	{
		freeblock1(inodearray[t].blocknum[10]);
	}
	if (inodearray[t].blocknum[11] > 0)
	{
		freeblock2(inodearray[t].blocknum[11]);
	}
	if (inodearray[t].blocknum[12] > 0)
	{
		freeblock3(inodearray[t].blocknum[12]);
	}
}

int writeindex()
{
	FILE* fp;
	if ((fp = fopen(INDEX_FILE_NAME, "w")) == NULL)
	{
		return -1;
	}
	fprintf(fp, "%d\n", phdinode->number);
	struct dinode* pt;
	pt = phdinode->next;
	//循环读取文件的每一行数据
	while (pt != NULL)
	{
		fprintf(fp, "%d %s\n", pt->number, pt->fileName);
		pt = pt->next;
	}
	for (int i = 0; i < DIRTABLE_MAX_SIZE; i++)
	{
		fprintf(fp, "%d %s %s %d %d %lld %lld %lld", inodearray[i].mode, inodearray[i].type, inodearray[i].username, inodearray[i].fileSize, inodearray[i].dataSize, inodearray[i].visittime, inodearray[i].changetime, inodearray[i].createtime);
		for (int j = 0; j < INDEX_NUM; j++)
		{
			fprintf(fp, " %hd", inodearray[i].blocknum[j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}

struct dinode* get_phdinode()
{
	return phdinode;
}

struct inode* get_inodearray(int t)
{
	return &(inodearray[t]);
}

void init_phdinode()
{
	phdinode = (struct dinode*)malloc(sizeof(struct dinode));
	phdinode->number = 0;
	phdinode->next = NULL;
}

int check_filename(char* str,char* type)
{
	struct dinode* p = phdinode->next;
	while (p != NULL)
	{
		if (!strcmp(p->fileName, str))
		{
			strcpy(type, inodearray[p->number].type);
			return 0;
		}
		p = p->next;
	}
	return 1;
}

struct dir *read_dir(struct inode * p)
{
	char* str = get_block(p->blocknum[0]);
	struct dir tempdir;
	memcpy(&tempdir, str,sizeof(struct dir));
	struct dir* readdir = (struct dir*)malloc(sizeof(struct dir) * (tempdir.num + YULIUXIANG_NUM));
	get_blocks_d(readdir,p, tempdir.num + 1);
	return readdir;
}

void write_dir(struct inode* p, struct dir* pdir)
{
	write_blocks_d(p, pdir);
	free(pdir);
}

char* oread_file(struct inode *p,int n)
{
	n = n < 0 || p->dataSize < n ? p->dataSize : n;
	char* readfile = (char*)malloc(sizeof(char) * (n + YULIUXIANG_FILE_NUM));
	get_blocks_f(readfile, p, 0, n + 1);
	readfile[n] = '\0';
	return readfile;
}

char* rread_file(struct inode *p,int ps,int pn)
{
	ps = ps < 0 ? 0 : (ps > p->dataSize ? p->dataSize : ps);
	pn = pn < 0 || p->dataSize  < ps + pn ? p->dataSize - ps : pn;
	//printf("%d %d %d\n", ps, pn,p->dataSize);
	char* readfile = (char*)malloc(sizeof(char) * (pn + YULIUXIANG_FILE_NUM));
	get_blocks_f(readfile, p, ps, ps + pn + 1);
	readfile[pn] = '\0';
	return readfile;
}

void write_file(struct inode* p, char* str ,int ps)
{
	ps = ps < 0 || ps > p->dataSize ? p->dataSize : ps;
	write_blocks_f(str,p,ps);
}

void get_time(char *str1,char *str2)
{
	strcpy(str1, str2);
	str1[strlen(str1) - 1] = '\0';
}

void showinodearray()
{
	char changetime[50],visittime[50],createtime[50];
	printf("文件权限 文件类型        拥有者姓名      盘块大小 数据大小 访问时间                  修改时间                  创建时间                    0    1    2    3    4    5    6    7    8    9   10   11   12   13 \n");
	for (int i = 0; i < DIRTABLE_MAX_SIZE; i++)
	{
		get_time(visittime, asctime(gmtime(&inodearray[i].visittime)));
		get_time(changetime, asctime(gmtime(&inodearray[i].changetime)));
		get_time(createtime, asctime(gmtime(&inodearray[i].createtime)));
		
		printf("%8d %15s %15s %8d %8d %25s %25s %25s", inodearray[i].mode, inodearray[i].type, inodearray[i].username, inodearray[i].fileSize, inodearray[i].dataSize, visittime, changetime, createtime);
		for (int j = 0; j < INDEX_NUM; j++)
		{
			printf(" %4hd", inodearray[i].blocknum[j]);
		}
		printf("\n");
	}
}
