#pragma warning(disable:4996)

#include "file.h"
#include "block.h"
#include "index.h"

#define NUMREADER 5

//struct dirTable* currentDirTable;  //��ǰλ��
char path[FILENAME_MAX_SIZE * 10];             //���浱ǰ����·��
char nowusername[USERNAME_MAX_SIZE] = "gjy135136";

void ls()
{
	int t = get_filenum_inodearray(path);
	struct inode* pinode = get_inodearray(t);
	struct dir* pdir;
	pdir = read_dir(pinode);
	for (int i = 1; i <= pdir[0].num; i++)
	{
		printf("%s%t\n", pdir[i].name);
	}
	free(pdir);
}

void rmdir(char* str)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	int t = get_filenum_inodearray(tempath);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath,type))
	{
		printf("%s�ļ��в�����\n",str);
		return;
	}
	if (strcmp(type, "filefolder") != 0)
	{
		printf("%s�����ļ���\n", str);
		return;
	}
	struct inode* pinode = get_inodearray(t);
	struct dir* pdir = read_dir(pinode);
	if (pdir[0].num)
	{
		printf("�޷�ɾ���ǿ��ļ���\n");
		free(pdir);
		return;
	}
	free(pdir);
	freeindex(t);
	move_filenum_inodearry(tempath);

	struct dinode* phdinode = get_phdinode();
	phdinode->number--;
	t = get_filenum_inodearray(path);
	pinode = get_inodearray(t);

	pdir = read_dir(pinode);
	for (int i = 1; i <= pdir[0].num; i++)
	{
		if (!strcmp(pdir[i].name, str))
		{
			strcpy(pdir[i].name, pdir[pdir[0].num--].name);
			break;
		}
	}
	write_dir(pinode,pdir);
}

void mkdir(char* str)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath,type))
	{
		short t = creatDir(tempath);
		int p = get_filenum_inodearray(path);
		struct inode* pinode = get_inodearray(p);
		struct dir* pdir;
		pdir = read_dir(pinode);
		pdir[++pdir[0].num].num = t;
		strcpy(pdir[pdir[0].num].name, str);
		write_dir(pinode,pdir);
	}
	else
	{
		printf("%s�ļ����Ѵ���\n", str);
	}
}

int creatDir(char* str)
{
	struct dinode* pt = (struct dinode*)malloc(sizeof(struct dinode));
	strcpy((pt->fileName),str);
	pt->number = get_freenum_inodearray();
	struct dinode* phdinode = get_phdinode();
	phdinode->number++;
	pt->next = phdinode->next;
	phdinode->next = pt;
	
	struct inode* pinode = get_inodearray(pt->number);
	pinode->blocknum[0] = get_freenum_blocks();
	struct dir* blockp = get_block(pinode->blocknum[0]);
	blockp[0].num = 0;
	pinode->mode = 0666;
	strcpy(pinode->type, "filefolder");
	strcpy(pinode->username, nowusername);
	for (int i = 1; i < INDEX_NUM; i++)
	{
		pinode->blocknum[i] = 0;
	}
	pinode->fileSize = 1;
	pinode->dataSize = 0;
	time(&(pinode->visittime));
	time(&(pinode->createtime));
	time(&(pinode->changetime));

	return pt->number;
}

//��ʼ����Ŀ¼
void initRootDir()
{
	char str[10] = "~";
	creatDir(str);
}

//��ʼ���ļ�ϵͳ
int init()
{
	init_phdinode();
	clear_inodearray();
	if (init_index() + init_block() < 0)
	{
		printf("��ӡ���ļ�ʧ��");
		return -1;
	}
	
	char* strc = get_blockarray(1);
	*strc = 1;

	//��ʼ����ʼ����·��
	path[0] = '~';
	path[1] = '\0';
	return 0;
}

//��þ���·��
char* getPath()
{
	return path;
}

//��ʽ���ļ�ϵͳ
void format()
{
	clear_inodearray();
	clear_blockarray();
	initRootDir();
	path[0] = '~';
	path[1] = '\0';
}

void cd(char* str)
{
	struct dinode* phd = get_phdinode();
	phd = phd->next;
	while (phd != NULL)
	{
		if (!strcmp(phd->fileName, str))
		{
			struct inode* pinode = get_inodearray(phd->number);
			if (!strcmp(pinode->type, "filefolder"))
			{
				strcpy(path, str);
			}
			else
			{
				printf("%s�����ļ���\n", str);
			}
			return;
		}
		phd = phd->next;
	}
	printf("%s������\n", str);
}

void create(char* str)
{
	char tempfile[100];
	sprintf(tempfile, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempfile,type))
	{
		struct dinode* pt = (struct dinode*)malloc(sizeof(struct dinode));
		strcpy(pt->fileName, tempfile);
		struct dinode* phdinode = get_phdinode();
		pt->number = get_freenum_inodearray();
		phdinode->number++;
		pt->next = phdinode->next;
		phdinode->next = pt;

		int p = get_filenum_inodearray(path);
		struct inode* pinode = get_inodearray(p);
		struct dir* pdir;
		pdir = read_dir(pinode);
		pdir[++pdir[0].num].num = pt->number;
		strcpy(pdir[pdir[0].num].name, str);
		write_dir(pinode, pdir);

		pinode = get_inodearray(pt->number);
		pinode->blocknum[0] = get_freenum_blocks();
		pinode->mode = 0666;
		strcpy(pinode->type, "file");
		strcpy(pinode->username, nowusername);
		for (int i = 1; i < INDEX_NUM; i++)
		{
			pinode->blocknum[i] = 0;
		}
		pinode->fileSize = 1;
		pinode->dataSize = 0;
		time(&(pinode->visittime));
		time(&(pinode->createtime));
		time(&(pinode->changetime));
	}
	else
	{
		printf("%s�ļ��Ѵ���\n", str);
	}
}

void open(char *str)
{
	/*���ļ�*/
}

void close(char* str)
{
	/*ɾ���ļ�*/
}

void rm(char* str)
{
	char tempfile[100];
	sprintf(tempfile, "%s/%s", path, str);
	struct dinode* p, * pt;
	p = get_phdinode();
	pt = p->next;
	while (pt != NULL)
	{
		if (!strcmp(pt->fileName, tempfile))
		{
			break;
		}
		p = pt;
		pt = pt->next;
	}
	if (pt == NULL)
	{
		printf("%s�ļ�������\n", str);
		return;
	}
	struct inode* tempinode = get_inodearray(pt->number);
	if (strcmp(tempinode->type, "file") != 0)
	{
		printf("%s�����ļ�\n", str);
		return;
	}
	struct dinode* phdinode = get_phdinode();
	phdinode->number--;
	p->next = pt->next;
	freeindex(pt->number);
	free(pt);

	int ps = get_filenum_inodearray(path);
	struct inode* pinode = get_inodearray(ps);
	struct dir* pdir;
	pdir = read_dir(pinode);
	for (int i = 1; i <= pdir[0].num; i++)
	{
		if (!strcmp(pdir[i].name, str))
		{
			pdir[i].num = pdir[pdir[0].num].num;
			strcpy(pdir[i].name,pdir[pdir[0].num].name);
			pdir[0].num--;
			break;
		}
	}
	write_dir(pinode, pdir);
}

void show(char* str)
{
	char tempfile[100];
	sprintf(tempfile, "%s/%s", path, str);
	int p = get_filenum_inodearray(tempfile);
	struct inode* point = get_inodearray(p);
	printf("�ļ���%s���ԣ�\n", str);
	printf("mode:%d\n", point->mode);
	printf("type:%s\n", point->type);
	printf("username:%s\n", point->username);
	printf("blocknum:�����ڵ�|�̿��\n");
	for (int i = 0; i < INDEX_NUM; i++)
	{
		printf("%t%t%d|%hd\n", i, point->blocknum[i]);
	}
	printf("fileSize:%d\n", point->fileSize);
	printf("dataSize:%d\n", point->dataSize);
	printf("visittime:%s\n", asctime(gmtime(&point->visittime)));
	printf("createtime:%s\n", asctime(gmtime(&point->createtime)));
	printf("changetime:%s\n", asctime(gmtime(&point->changetime)));
}

void changename(char *str,char *str2)
{
	char tempfile[100];
	sprintf(tempfile, "%s/%s", path, str);
	char changefile[100];
	sprintf(changefile, "%s/%s", path, str2);
	struct dinode* p, * pt;
	p = get_phdinode();
	pt = p->next;
	while (pt != NULL)
	{
		if (!strcmp(pt->fileName, changefile))
		{
			break;
		}
		pt = pt->next;
	}
	if (pt != NULL)
	{
		printf("%s�Ѵ���\n", str2);
		return;
	}

	pt = p->next;
	while (pt != NULL)
	{
		if (!strcmp(pt->fileName, tempfile))
		{
			break;
		}
		p = pt;
		pt = pt->next;
	}
	if (pt == NULL)
	{
		printf("%s������\n", str);
		return;
	}
	struct inode* tempinode = get_inodearray(pt->number);
	struct dinode* phdinode = get_phdinode();
	strcpy(pt->fileName,changefile);

	int ps = get_filenum_inodearray(path);
	struct inode* pinode = get_inodearray(ps);
	struct dir* pdir;
	pdir = read_dir(pinode);
	for (int i = 1; i <= pdir[0].num; i++)
	{
		if (!strcmp(pdir[i].name, str))
		{
			strcpy(pdir[i].name, str2);
			break;
		}
	}
	write_dir(pinode, pdir);
}

/*int get_block_num(struct inode* point, int* type)
{
	int addr = 0;
	int block_size = get_block_size();
	for (int i = 0; i < INDEX_NUM; i++)
	{
		if (point->blocknum[i] != 0)
		{
			addr = i;
			break;
		}
	}
	if (addr < 10)
	{
		return addr - 1;
	}
	else if (addr == 10)
	{
		return 9 + point->blocknum[10];
	}
	else if (addr == 11)
	{
		return 9 + block_size + point->blocknum[11];
	}
	else
	{
		return 9 + block_size + block_size * block_size + point->blocknum[12];
	}
}

char* get_file(struct inode* point)
{
	int type;
	int block_num = get_block_num(point, &type);
	int addr = 0;
	int block_size = get_block_size();
	for (int i = 0; i < INDEX_NUM; i++)
	{
		if (point->blocknum[i] != 0)
		{
			addr = i;
			break;
		}
	}
	if (addr < 10)
	{
		return getBlock(addr);
	}
	else if (addr == 10)
	{

		return 9 + point->blocknum[10];
	}
	else if (addr == 11)
	{
		return 9 + block_size + point->blocknum[11];
	}
	else
	{
		return 9 + block_size + block_size * block_size + point->blocknum[12];
	}
}
*/

void exitsys()
{
	path[0] = '~';
	path[1] = '\0';
	int t = writeindex() + writeblock();
	if (t < 0)
	{
		printf("/*******�˳��ļ�ϵͳ����******/");
	}
	else
	{
		printf("/*******�ɹ��˳��ļ�ϵͳ******/");
	}
}

void ordereader(char *str,int n)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath, type))
	{
		printf("%s�ļ�������\n", str);
		return;
	}
	if (strcmp(type, "file") != 0)
	{
		printf("%s�����ļ�\n", str);
		return;
	}
	int p = get_filenum_inodearray(tempath);
	struct inode* pinode = get_inodearray(p);
	char* pfile = oread_file(pinode,n);
	printf("%s\n", pfile);
	free(pfile);
	time(&(pinode->visittime));
}

void randomreader(char* str, int t, int p)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath, type))
	{
		printf("%s�ļ�������\n", str);
		return;
	}
	if (strcmp(type, "file") != 0)
	{
		printf("%s�����ļ�\n", str);
		return;
	}
	int ps = get_filenum_inodearray(tempath);
	struct inode* pinode = get_inodearray(ps);
	char* pfile = rread_file(pinode,t,p);
	printf("%s\n", pfile);
	free(pfile);
	time(&(pinode->visittime));
}

void orderwriter(char* str)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath, type))
	{
		printf("%s�ļ�������\n", str);
		return;
	}
	if (strcmp(type, "file") != 0)
	{
		printf("%s�����ļ�\n", str);
		return;
	}
	char* command = (char*)malloc(sizeof(char) * YULIUXIANG_WRITE_NUM);
	char c;
	int n = 0, t = YULIUXIANG_WRITE_NUM - 1;
	while ((c = getchar()) != '$')
	{
		command[n++] = c;
		if (n >= t)
		{
			char* q = (char*)realloc(command,sizeof(char) * (n + YULIUXIANG_WRITE_NUM));
			t = n + YULIUXIANG_WRITE_NUM - 1;
			if (command != q)
			{
				free(command);
				command = q;
			}
		}
	}
	rewind(stdin);
	command[n] = '\0';
	int p = get_filenum_inodearray(tempath);
	struct inode* pinode = get_inodearray(p);
	char* pfile = oread_file(pinode, -1);
	//printf("%s\n", pfile);
	char* q = (char*)realloc(command, sizeof(char) * (pinode->dataSize + n + YULIUXIANG_FILE_NUM));
	if (command != q)
	{
		free(command);
		command = q;
	}
	strcat(command,pfile);
	free(pfile);
	write_file(pinode,command,0);
	pinode->dataSize = strlen(command);
	free(command);
	time(&(pinode->visittime));
	time(&(pinode->changetime));
}

void randomwriter(char* str, int s)
{
	char tempath[100];
	sprintf(tempath, "%s/%s", path, str);
	char type[TYPE_MAX_SIZE];
	if (check_filename(tempath,type))
	{
		printf("%s�ļ�������\n", str);
		return;
	}
	if (strcmp(type, "file") != 0)
	{
		printf("%s�����ļ�\n",str);
		return;
	}
	char* command = (char*)malloc(sizeof(char) * YULIUXIANG_WRITE_NUM);
	char c;
	int n = 0, t = YULIUXIANG_WRITE_NUM - 1;
	while ((c = getchar()) != '$')
	{
		command[n++] = c;
		if (n >= t)
		{
			char* q = (char*)realloc(command, sizeof(char) * (n + YULIUXIANG_WRITE_NUM));
			t = n + YULIUXIANG_WRITE_NUM - 1;
			if (command != q)
			{
				free(command);
				command = q;
			}
		}
	}
	rewind(stdin);
	command[n] = '\0';
	int p = get_filenum_inodearray(tempath);
	struct inode* pinode = get_inodearray(p);
	char* pfile = rread_file(pinode, s, -1);
	//printf("%s\n", pfile);
	char* q = (char*)realloc(command, sizeof(char) * (pinode->dataSize + n + YULIUXIANG_FILE_NUM));
	if (command != q)
	{
		free(command);
		command = q;
	}
	strcat(command, pfile);
	//printf("%s", command);
	free(pfile);
	write_file(pinode, command, s);
	pinode->dataSize = s + strlen(command);
	free(command);
	time(&(pinode->visittime));
	time(&(pinode->changetime));
}

void showinode()
{
	printf("�ļ�Ŀ¼�\n");
	struct dinode* pd = get_phdinode();
	printf("sum:%d\n", pd->number);
	pd = pd->next;
	while (pd != NULL)
	{
		printf("%5d %20s\n", pd->number, pd->fileName);
		pd = pd->next;
	}
	printf("�ļ�������\n");
	showinodearray();
}

void changeblocksize(int blocksize)
{
	if (blocksize <= 0 || blocksize > BLOCK_MAX_SIZE)
	{
		printf("�Ƿ��̿��С(1~1024)\n");
		return;
	}
	struct dinode* pt = get_phdinode();
	pt = pt->next;
	struct tdinode* pd = (struct tdinode*)malloc(sizeof(struct tdinode));
	struct tdinode* pnow = pd;
	while (pt != NULL)
	{
		struct tdinode* pnew = (struct tdinode*)malloc(sizeof(struct tdinode));
		pnew->number = pt->number;
		struct inode* pinode = get_inodearray(pt->number);
		if (!strcmp(pinode->type, "file"))
		{
			pnew->t = 2;
			pnew->str = oread_file(pinode, -1);
			//printf("%s\n", pnew->str);
		}
		else if(!strcmp(pinode->type, "filefolder"))
		{
			pnew->t = 1;
			pnew->fdir = read_dir(pinode);
		}
		pnow->next = pnew;
		pnow = pnew;
		pt = pt->next;
	}
	pnow->next = NULL;
	chbls(blocksize);
	pnow = pd->next;
	free(pd);
	while (pnow != NULL)
	{
		pd = pnow;
		struct inode* pinode = get_inodearray(pnow->number);
		if (pnow->t == 2)
		{
			//printf("%s\n", pnow->str);
			write_file(pinode, pnow->str, 0);
		}
		else if (pnow->t == 1)
		{
			write_dir(pinode, pnow->fdir);
		}
		pnow = pnow->next;
		free(pd);
	}
}
