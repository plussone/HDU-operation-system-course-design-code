#pragma warning(disable:4996)

#include "block.h"
#include "file.h"
#include "index.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int system_size = 1024 * 1024;        //系统大小
int block_size = 1024;  //盘块大小
int block_count = 1024; //系统盘块数目
int dir_num = 50;
int block_num = 50;

char blocks[BLOCK_MAX_NUM][BLOCK_MAX_SIZE];
char blockarray[BLOCK_MAX_NUM];

char* get_blockarray(int t)
{
	return &(blockarray[t]);
}

void clear_blockarray()
{
	for (int i = 0; i < block_count; i++)
	{
		blockarray[i] = 0;
		blocks[i][0] = '\0';
	}
}

int get_freenum_blocks()
{
	for (int i = 1; i < block_count; i++)
	{
		if (blockarray[i] == 0)
		{
			blockarray[i] = 1;
			return i;
		}
	}
	printf("剩余磁盘空间不足\n");
	return -1;
}

int writeblock()
{
	FILE* fp;
	if ((fp = fopen(BLOCK_FILE_NAME, "w")) == NULL)
	{
		return -1;
	}
	fprintf(fp, "Filesystem-made-by-plussone.welcome-to-using!\n");
	fprintf(fp, "%d\n", system_size);
	fprintf(fp, "%d\n", block_size);
	fprintf(fp, "%d\n", block_count);
	fprintf(fp, "%d\n", dir_num);
	fprintf(fp, "%d\n", block_num);
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		fprintf(fp, "%d\n", blockarray[i]);
	}
	for (int i = 0; i < BLOCK_MAX_NUM;i++)
	{
		for (int j = 0; j < BLOCK_MAX_SIZE; j++)
		{
			fprintf(fp, "%c", blocks[i][j]);
		}
	}
	fclose(fp);
	return 0;
}

int init_block()
{
	FILE* fp;
	if ((fp = fopen(BLOCK_FILE_NAME, "r")) == NULL)
	{
		return -1;
	}
	char logs[1024];
	fscanf(fp, "%s", logs);
	fscanf(fp, "%d", &system_size);
	fscanf(fp, "%d", &block_size);
	fscanf(fp, "%d", &block_count);
	fscanf(fp, "%d", &dir_num);
	fscanf(fp, "%d", &block_num);
	printf("%s\n",logs);
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		fscanf(fp, "%d", &blockarray[i]);
	}
	char s;
	fscanf(fp, "%c", &s);
	for (int i = 0; i < BLOCK_MAX_NUM; i++)
	{
		for (int j = 0; j < BLOCK_MAX_SIZE; j++)
		{
			fscanf(fp, "%c", &blocks[i][j]);
		}
	}
	fclose(fp);
	return 0;
}

void freeblock0(int t)
{
	blockarray[t] = 0;
	blocks[t][0] = '\0';
}

void freeblock1(int t)
{
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] > 0)
		{
			freeblock0(in[i]);
		}
	}
}

void freeblock2(int t)
{
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] > 0)
		{
			freeblock1(in[i]);
		}
	}
}

void freeblock3(int t)
{
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] > 0)
		{
			freeblock2(in[i]);
		}
	}
}

char* get_block(int t)
{
	return blocks[t];
}

struct dir* getblock0d(int* pt, int t, struct dir* rdir)
{
	memcpy(rdir, blocks[t], dir_num * sizeof(struct dir));
	*pt -= dir_num;
	return rdir + dir_num;
}

struct dir* getblock1d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock0d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return tp;
}

struct dir* getblock2d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock1d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return tp;
}

struct dir* getblock3d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock2d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return rdir;
}

void get_blocks_d(struct dir* rdir, struct inode* p, int n)
{
	struct dir* tp = rdir;
	int sn = n;
	for (int i = 0; i < 10; i++)
	{
		if (sn > 0 && p->blocknum[i])
		{
			tp = getblock0d(&sn, p->blocknum[i], tp);
		}
	}
	if (sn > 0 && p->blocknum[10])
	{
		tp = getblock1d(&sn, p->blocknum[10], tp);
	}
	if (sn > 0 && p->blocknum[11])
	{
		tp = getblock2d(&sn, p->blocknum[11], tp);
	}
	if (sn > 0 && p->blocknum[12])
	{
		tp = getblock3d(&sn, p->blocknum[12], tp);
	}
}

struct dir* writeblock0d(int* pt, int t, struct dir* rdir)
{
	memcpy(blocks[t], rdir, dir_num * sizeof(struct dir));
	*pt -= dir_num;
	return rdir + dir_num;
}

struct dir* writeblock1d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] = 0)
		{
			in[i] = get_freenum_blocks();
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = getblock0d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return tp;
}

struct dir* writeblock2d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] = 0)
		{
			in[i] = get_freenum_blocks();
			clear_block_index(in[i]);
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = getblock1d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return tp;
}

struct dir* writeblock3d(int* pt, int t, struct dir* rdir)
{
	if (*pt <= 0)
	{
		return rdir;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	struct dir* tp = rdir;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] = 0)
		{
			in[i] = get_freenum_blocks();
			clear_block_index(in[i]);
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = getblock2d(pt, in[i], tp);
		if (*pt <= 0)
		{
			break;
		}
	}
	return tp;
}

void write_blocks_d(struct inode* p, struct dir* pdir)
{
	struct dir* tp = pdir;
	int sn = pdir->num + 1;
	for (int i = 0; i < 10; i++)
	{
		if (sn > 0)
		{
			if (p->blocknum[i] == 0)
			{
				p->blocknum[i] = get_freenum_blocks();
			}
			tp = writeblock0d(&sn, p->blocknum[i], tp);
		}
	}
	if (sn > 0)
	{
		if (p->blocknum[10] == 0)
		{
			p->blocknum[10] = get_freenum_blocks();
			clear_block_index(p->blocknum[10]);
		}
		tp = writeblock1d(&sn, p->blocknum[10], tp);
	}
	if (sn > 0)
	{
		if (p->blocknum[11] == 0)
		{
			p->blocknum[11] = get_freenum_blocks();
			clear_block_index(p->blocknum[11]);
		}
		tp = writeblock2d(&sn, p->blocknum[11], tp);
	}
	if (sn > 0)
	{
		if (p->blocknum[12] == 0)
		{
			p->blocknum[12] = get_freenum_blocks();
			clear_block_index(p->blocknum[11]);
		}
		tp = writeblock3d(&sn, p->blocknum[12], tp);
	}
}

void clear_block_index(int t)
{
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	for (int i = 0; i < block_num; i++)
	{
		in[i] = 0;
	}
	memcpy(blocks[t], in, sizeof(int) * block_num);
}

void get_blocks_f(char* rfile, struct inode* p, int s, int n)
{
	char* tp = rfile;
	int sns = s, snn = n;
	for (int i = 0; i < 10; i++)
	{
		if (snn > 0 && p->blocknum[i])
		{
			tp = getblock0f(&sns, &snn, p->blocknum[i], tp);
		}
	}
	if (snn > 0 && p->blocknum[10])
	{
		tp = getblock1f(&sns, &snn, p->blocknum[10], tp);
	}
	if (snn > 0 && p->blocknum[11])
	{
		tp = getblock2f(&sns, &snn, p->blocknum[11], tp);
	}
	if (snn > 0 && p->blocknum[12])
	{
		tp = getblock3f(&sns, &snn, p->blocknum[12], tp);
	}
}

char* getblock0f(int* ps, int* pn, int t, char* rfile)
{
	if (*ps < block_size)
	{
		if (*ps > 0)
		{
			if (*pn > block_size)
			{
				memcpy(rfile, blocks[t] + *ps, sizeof(char) * (block_size - (*ps)));
			}
			else
			{
				memcpy(rfile, blocks[t] + *ps, sizeof(char) * ((*pn) - (*ps)));
			}
		}
		else
		{
			if (*pn > block_size)
			{
				memcpy(rfile, blocks[t], block_size * sizeof(char));
			}
			else
			{
				memcpy(rfile, blocks[t], (*pn) * sizeof(char));
			}
		}
	}
	*ps -= block_size;
	*pn -= block_size;
	return rfile + block_size;
}

char* getblock1f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock0f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

char* getblock2f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock1f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

char* getblock3f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		tp = getblock2f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

void write_blocks_f(char* str, struct inode* p, int s)
{
	char* tp = str;
	int sns = s, snn = s + strlen(str);
	for (int i = 0; i < 10; i++)
	{
		if (snn > 0)
		{
			if (p->blocknum[i] == 0)
			{
				p->blocknum[i] = get_freenum_blocks();
			}
			tp = writeblock0f(&sns, &snn, p->blocknum[i], tp);
		}
	}
	if (snn > 0)
	{
		if (p->blocknum[10] == 0)
		{
			p->blocknum[10] = get_freenum_blocks();
			clear_block_index(p->blocknum[10]);
		}
		tp = writeblock1f(&sns, &snn, p->blocknum[10], tp);
	}
	if (snn > 0)
	{
		if (p->blocknum[11] == 0)
		{
			p->blocknum[11] = get_freenum_blocks();
			clear_block_index(p->blocknum[11]);
		}
		tp = writeblock2f(&sns, &snn, p->blocknum[11], tp);
	}
	if (snn > 0)
	{
		if (p->blocknum[12] == 0)
		{
			p->blocknum[12] = get_freenum_blocks();
			clear_block_index(p->blocknum[12]);
		}
		tp = writeblock3f(&sns, &snn, p->blocknum[12], tp);
	}
}

char* writeblock0f(int* ps, int* pn, int t, char* rfile)
{
	if (*ps < block_size)
	{
		if (*ps > 0)
		{
			if (*pn > block_size)
			{
				memcpy(blocks[t] + *ps, rfile, sizeof(char) *  (block_size - (*ps)));
			}
			else
			{
				memcpy(blocks[t] + *ps, rfile, sizeof(char) * ((*pn) - (*ps)));
			}
		}
		else
		{
			if (*pn > block_size)
			{
				memcpy(blocks[t], rfile, block_size * sizeof(char));
			}
			else
			{
				memcpy(blocks[t], rfile, (*pn) * sizeof(char));
			}
		}
	}
	*ps -= block_size;
	*pn -= block_size;
	return rfile + block_size;
}

char* writeblock1f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] == 0)
		{
			in[i] = get_freenum_blocks();
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = writeblock0f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

char* writeblock2f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	memcpy(in, blocks[t], sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] == 0)
		{
			in[i] = get_freenum_blocks();
			clear_block_index(in[i]);
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = writeblock1f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

char* writeblock3f(int* ps, int* pn, int t, char* rfile)
{
	if (*pn <= 0)
	{
		return rfile;
	}
	int* in = (int*)malloc(sizeof(int) * block_num);
	char* tp = rfile;
	for (int i = 0; i < block_num; i++)
	{
		if (in[i] == 0)
		{
			in[i] = get_freenum_blocks();
			clear_block_index(in[i]);
			memcpy(blocks[t], in, sizeof(int) * block_num);
		}
		tp = writeblock2f(ps, pn, in[i], tp);
		if (*pn <= 0)
		{
			break;
		}
	}
	return tp;
}

void chbls(int blocksize)
{
	block_size = blocksize; 
	block_count = system_size / block_size > BLOCK_MAX_NUM ? BLOCK_MAX_NUM : system_size / block_size;
	dir_num = block_size / sizeof(struct dir);
	block_num = block_size / sizeof(int);
}
