                             #include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
#include <string.h>

//ͳ���鱾����
static int get_booknode_count(BookNode *book_list)
{
	int ret=0;

	while(book_list)
	{
		ret++;
		book_list=book_list->next;
	}
	return ret;
}

//ͳ�ƽ�������
static int get_borrowernode_count(BorrowerNode * borrower_list)
{
	int ret=0;
	
	while(borrower_list)
	{
		ret++;
		borrower_list=borrower_list->next;
	}
	return ret;
}

//����ͼ��ռ�
static BookNode * alloc_booknode()
{
	BookNode * tmp=(BookNode *)malloc(sizeof(BookNode));
	assert(tmp);             //����һ�������������ǳ�����ֹ
	tmp->book.borrowed=0;
	tmp->book.borrower[0]='\0';
	tmp->prev=NULL;
	tmp->next=NULL;
	return tmp;
}

//��������߿ռ�
static BorrowerNode * alloc_borrowernode()
{
	BorrowerNode *tmp=(BorrowerNode *)malloc(sizeof(BorrowerNode));
	assert(tmp);
	tmp->borrower.lendnum=0;
	tmp->prev=NULL;
	tmp->next=NULL;
	return tmp;
}

//�ⲿ���������ͷ�ͼ��ڵ��ڴ���
static void free_booknode(BookNode *node)
{
	free(node);
}

//�ͷŽ����߽ӵ��ڴ���
static void free_borrowernode(BorrowerNode *node)
{
	free(node);
}

//�������ļ���������
static int load_all_data()
{
	FILE *fp;
	BookNode *book_node=NULL;
	BorrowerNode *borrower_node=NULL;
	int node_count;
	fp=fopen(DATA_FILE_NAME,"rb");//��ȡ����
	if(!fp)
		return -1;
	fread(&node_count,sizeof(node_count),1,fp);
	while(node_count)
	{
		BorrowerNode * tmp;            //��������
		tmp=alloc_borrowernode();            //����һ�������߿ռ�
		fread(&tmp->borrower,sizeof(tmp->borrower),1,fp);

		if(borrower_node)
		{
			borrower_node->next=tmp;
			tmp->prev=borrower_node;
		}
		else
		{
			g_borrower_list=tmp;
		}
		borrower_node=tmp;
		node_count--;
	}
	if(g_borrower_list)
		g_borrower_list->prev=borrower_node;
	fread(&node_count,sizeof(node_count),1,fp);
	while(node_count)
	{
		BookNode *tmp;
		tmp=alloc_booknode();
		fread(&tmp->book,sizeof(tmp->book),1,fp);
		if(book_node)
		{
			book_node->next=tmp;
			tmp->prev=book_node;

		}
		else
		{
			g_book_list=tmp;
		}
		book_node=tmp;
		node_count--;
	}
	if(g_book_list)
		g_book_list->prev=book_node;
	fclose(fp);
	return 0;
}


//�洢�������ݵ������ļ�
static int save_all_data()
{
	FILE *fp;
	BookNode *book_node=g_book_list;
	BorrowerNode *borrower_node=g_borrower_list;
	int node_count;
	fp=fopen(DATA_FILE_NAME,"wb");
	if(!fp)
		return -1;
	//��д�����
	node_count=get_borrowernode_count(borrower_node);
	fwrite(&node_count,sizeof(node_count),1,fp);
	while(borrower_node)
	{
		fwrite(&borrower_node->borrower,sizeof(borrower_node->borrower),1,fp);
		borrower_node=borrower_node->next;
	}
	node_count=get_booknode_count(book_node);
	fwrite(&node_count,sizeof(node_count),1,fp);
	while(book_node)
	{
		fwrite(&book_node->book,sizeof(book_node->book),1,fp);
		book_node=book_node->next;
	}
	fclose(fp);
	return 0;
}

//��ʾͼ����Ϣ
static void display_book(Book *bo)
{
	assert(bo);
	printf("\t ������%s\n",bo->name);
	printf("\t ���ߣ�&s\n",bo->author);
}

static void display_borrower(Borrower *br)      //��ʾ������Ϣ
{
	assert(br);
	printf("\t ������%s\n",br->name);
	printf("\t �Ա�%s\n",br->sex);
	printf("\t סַ��%s\n",br->address);

}
int init_system(void) //��ʼ��ϵͳ
{
	if(load_all_data())
		return -1;
	return 0;
}


//�ͷ�ϵͳ�е����ݿռ�
void release_system()
{
	BookNode *book_node=g_book_list;
	BorrowerNode *borrower_node=g_borrower_list;
	save_all_data();
	while(book_node)
	{
		BookNode *tmp=book_node->next;
		free_booknode(book_node);
		book_node=tmp;
	}
	while(borrower_node)
	{
		BorrowerNode *tmp=borrower_node->next;
		free_borrowernode(borrower_node);
		borrower_node=tmp;
	}
}

//���ҽ�����
static BorrowerNode *find_borrowernode(const char *name)
{
	BorrowerNode *borrower_node=g_borrower_list;
	while(borrower_node)
	{
		if(0==strcmp(borrower_node->borrower.name,name))
			return borrower_node;
		borrower_node=borrower_node->next;
	}
	return NULL;
}

//����ͼ��
static BookNode *find_booknode(const char *name)
{
	BookNode *book_node=g_book_list;
	while(book_node)
	{
		if(0==strcmp(book_node->book.name,name))
			return book_node;
		book_node=book_node->next;
	}
	return NULL;
}
//������������Ϣ
int create_borrower()
{
	BorrowerNode *p=alloc_borrowernode();
	char ch;
	puts("\n\t<<ע�������>>\n");
	printf("����������");
	gets(p->borrower.name);
	if(find_borrowernode(p->borrower.name))
	{
		puts("ϵͳ����ͬ���Ľ����ߴ���");
		goto lab_failed;
	}
	printf("�����Ա�");
	gets(p->borrower.sex);
	printf("�����ַ��");
	gets(p->borrower.address);
	printf("ȷ��ע��ý�����?(N/Y)");
	ch=getch();
	if(ch=='n'&&ch=='N')
		goto lab_failed;
	if(g_borrower_list)
	{
		g_borrower_list->prev->next=p;
		p->prev=g_borrower_list->prev;
		g_borrower_list->prev=p;
	}
	else
	{
		g_borrower_list=p;
		g_borrower_list->prev=g_borrower_list;
	}
	puts("\nע������߳ɹ�!");
	getch();
	return 0;
lab_failed:
	free_borrowernode(p);
	puts("\nע�������ʧ��!");
	getch();
	return -1;
}


//����ͼ����Ϣ
int create_book()
{
	BookNode *p=alloc_booknode();
	char ch;
	puts("\n\t<<ע��ͼ��>>\n");
	printf("����������");
	gets(p->book.name);
	if(find_booknode(p->book.name))
	{
		puts("ϵͳ����ͬ����ͼ�����");
		goto lab_failed;
	}
	printf("�������ߣ�");
	gets(p->book.author);
	printf("ȷ��ע��ͼ����?(Y/N)");
	ch=getch();
	if(ch=='n'&&ch=='N')
		goto lab_failed;
	if(g_book_list)
	{
		g_book_list->prev->next=p;
		p->prev=g_book_list->prev;
		g_book_list->prev=p;
	}
	else
	{
		g_book_list=p;
		g_book_list->prev=g_book_list;
	}
	puts("\nע��ͼ��ɹ�!");
	getch();
	return 0;
lab_failed:
	free_booknode(p);
	puts("\nע��ͼ��ʧ��!");
	getch();
	return -1;
}

//ɾ��������
int remove_borrower()
{
	char name[_MAX_STRLEN];
	char ch;
	BorrowerNode *p;
	puts("\n\t<<ע��������>>\n");
	printf("����������");
	gets(name);
	p=find_borrowernode(name);
	if(!p)
	{
		puts("ϵͳ�в����ڸý�����");
		goto lab_failed;
	}
	printf("ȷ��ע���ý�����?(Y/N)");
	ch=getch();
	if(ch=='n'&&ch=='N')
		goto lab_failed;
	if(g_borrower_list==p)
	{
		g_borrower_list=g_borrower_list->next;
		if(g_borrower_list)
			g_borrower_list->prev=p->prev;
	}
	else
	{
		p->prev->next=p->prev;
		if(p->next)
			p->next->prev=p->prev;
	}
	free_borrowernode(p);
	puts("\nע�������߳ɹ�!");
	getch();
	return 0;
lab_failed:
	puts("\nע��������ʧ��!");
	getch();
	return -1;
}

//ɾ��ͼ��
int remove_book()
{
	char name[_MAX_STRLEN];
	char ch;
	BookNode *p;
	puts("\n\t<<ע��ͼ��>>\n");
	printf("����������");
	gets(name);
	p=find_booknode(name);

	if(!p)
	{
		puts("ϵͳ�в����ڸ�ͼ��");
		goto lab_failed;
	}
	printf("ȷ��ע����ͼ��?(Y/N)");
	ch=getch();
	if(ch=='n'&&ch=='N')
		goto lab_failed;
	if(g_book_list==p)
	{
		p->prev->next=p->next;
		if(g_book_list)
			g_book_list->prev=p->prev;
	}
	else
	{
		p->prev->next=p->next;
		if(p->next)
			p->next->prev=p->prev;
	}
	free_booknode(p);
	puts("\nע��ͼ��ɹ�!");
	getch();
	return 0;
lab_failed:
	puts("\nע��ͼ��ʧ��!");
	getch();
	return -1;
	
}

//����
int lend_book()
{
	char name[_MAX_STRLEN];
	char ch;
	BorrowerNode *br_node;
	BookNode *bo_node;
	puts("\n\t<<����ͼ��>>\n");
	printf("���������������");
	gets(name);
	br_node=find_borrowernode(name);
	if(!br_node)
	{
		puts("ϵͳ�в����ڸý�����");
		goto lab_failed;
	}
	if(br_node->borrower.lendnum>=_MAX_LENDBOOK)
	{
		printf("�ý������ѽ���%d����\n",_MAX_LENDBOOK);
		goto lab_failed;
	}
	printf("����Ҫ��ͼ��������");
	gets(name);
	bo_node=find_booknode(name);
	if(!bo_node)
	{
		puts("ϵͳ�в����ڸ�ͼ��");
		goto lab_failed;
	}
	if(bo_node->book.borrowed)
	{
		printf("��ͼ���ѽ��%s\n",bo_node->book.borrower);
		goto lab_failed;
	}
	printf("ȷ������ͼ����?(Y/N)");
	ch=getch();
	if(ch=='N'&&ch=='n')
		goto lab_failed;

	strcmp(br_node->borrower.book[br_node->borrower.lendnum],bo_node->book.name);
	strcmp(bo_node->book.borrower,br_node->borrower.name);
	br_node->borrower.lendnum++;
	bo_node->book.borrowed=1;
	puts("\n����ͼ��ɹ�!");
	getch();
	return 0;
lab_failed:
	puts("\n����ͼ��ʧ��!");
	getch();
	return -1;

}
//����
int return_book()
{
	char name[_MAX_STRLEN];
	char ch;
	int i;
	BorrowerNode *br_node;
	BookNode *bo_node;
	puts("\n\t<<�黹ͼ��>>\n");
	printf("���뻹����������");
	gets(name);
	br_node=find_borrowernode(name);
	if(!br_node)
	{
		puts("ϵͳ�в����ڸû�����");
		goto lab_failed;
	}
	if(br_node->borrower.lendnum==0)
	{
		puts("����û�н���");
		goto lab_failed;
	}
	printf("����Ҫ��ͼ��������");
	gets(name);
	bo_node=find_booknode(name);
	if(!bo_node)
	{
		puts("ϵͳ�в����ڸ�ͼ��");
		goto lab_failed;
	}
	if(!bo_node->book.borrowed)
	{
		printf("��ͼ��û�б�����");
		goto lab_failed;
	}
	for(i=0;i<br_node->borrower.lendnum;i++)
	{
		if(0==strcmp(br_node->borrower.book[i],name))
			break;
	}
	if(i!=br_node->borrower.lendnum)
	{
		printf("��ͼ�鲻�Ǳ�%s���裬����Ϊ%s����\n",br_node->borrower.name,bo_node->book.borrower);
		goto lab_failed;
	}
	printf("ȷ���黹ͼ����?(Y/N)");
	ch=getch();
	if(ch=='N'&&ch=='n')
		goto lab_failed;
	br_node->borrower.lendnum--;
	if(i<br_node->borrower.lendnum)
	{
		strcmp(br_node->borrower.book[i],br_node->borrower.book[br_node->borrower.lendnum]);
	}
	br_node->borrower.book[br_node->borrower.lendnum][0]='\0';
	bo_node->book.borrower[0]='\0';
	bo_node->book.borrowed=0;
	puts("\n�黹ͼ��ɹ�!");
	getch();
	return 0;
lab_failed:
	puts("\n�黹ͼ��ʧ��!");
	getchar();
	return -1;
}

//��ʾ��������Ϣ
void display_borrower_info(Borrower *br)
{
	int  i;
	puts("==========================================");
	display_borrower(br);
	if(br->lendnum)
	{
		puts("&&&&&&&&&&&������Ϣ&&&&&&&&&&&&&");
		for(i=0;i<br->lendnum;i++)
		{
			BookNode *p;
			p=find_booknode(br->book[i]);
			if(p)
				display_book(&p->book);
			puts("****************************");
			
		}
	}

}

//ͼ����Ϣ
void display_book_info(Book *bo)\
{
	puts("===================================");
	display_book(bo);
	if(bo->borrowed)
		printf("\t���鱻%s���\n",bo->borrower);
}

//���ҽ�����
int lookup_borrower()
{
	BorrowerNode *p;
	char name[_MAX_STRLEN];
	puts("\n\t<<���ҽ�����>>");
	printf("����������");
	gets(name);
	if(!(p=find_borrowernode(name)))
	{
		puts("ϵͳ�в����ڸý�����");
		return -1;
	}
	display_borrower_info(&p->borrower);
	getch();
	return 0;
}
//����ͼ��
int lookup_book()
{
	BookNode *p;
	char name[_MAX_STRLEN];
	puts("\n\t<<����ͼ��>>");
	printf("����������");
	gets(name);
	if(!(p=find_booknode(name)))
	{
		puts("ϵͳ�в����ڸ�ͼ��");
		return -1;
	}
	display_book_info(&p->book);
	getch();
	return 0;
}
//������н�����
int browse_borrower()
{
	BorrowerNode *borrower_node=g_borrower_list;
	char ch;
	while(borrower_node)
	{
		display_borrower_info(&borrower_node->borrower);
		puts("���������ʾ��һ����������Ϣ����Q��ֹ���........");
		ch=getch();
		if(ch=='q'&&ch=='Q')
			return 0;
		borrower_node=borrower_node->next;
	}
	puts("ȫ����������Ϣ�������");
	getch();
	return 0;
}
//���ȫ��ͼ��
int browse_book()
{
	BookNode *book_node=g_book_list;
	char ch;
	while(book_node)
	{
		display_book_info(&book_node->book);
		puts("���������ʾ��һ��ͼ����Ϣ����Q��ֹ���........");
		ch=getch();
		if(ch=='q'&&ch=='Q')
			return 0;
		book_node=book_node->next;
	}
	puts("ȫ��ͼ����Ϣ�������");
	getch();
	return 0;
}