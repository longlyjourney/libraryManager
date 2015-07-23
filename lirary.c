                             #include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <conio.h>
#include <string.h>

//统计书本数量
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

//统计借阅人数
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

//分配图书空间
static BookNode * alloc_booknode()
{
	BookNode * tmp=(BookNode *)malloc(sizeof(BookNode));
	assert(tmp);             //测试一个条件并可能是程序终止
	tmp->book.borrowed=0;
	tmp->book.borrower[0]='\0';
	tmp->prev=NULL;
	tmp->next=NULL;
	return tmp;
}

//分配借阅者空间
static BorrowerNode * alloc_borrowernode()
{
	BorrowerNode *tmp=(BorrowerNode *)malloc(sizeof(BorrowerNode));
	assert(tmp);
	tmp->borrower.lendnum=0;
	tmp->prev=NULL;
	tmp->next=NULL;
	return tmp;
}

//外部声明函数释放图书节点内存区
static void free_booknode(BookNode *node)
{
	free(node);
}

//释放借阅者接点内存区
static void free_borrowernode(BorrowerNode *node)
{
	free(node);
}

//从数据文件加载数据
static int load_all_data()
{
	FILE *fp;
	BookNode *book_node=NULL;
	BorrowerNode *borrower_node=NULL;
	int node_count;
	fp=fopen(DATA_FILE_NAME,"rb");//读取类型
	if(!fp)
		return -1;
	fread(&node_count,sizeof(node_count),1,fp);
	while(node_count)
	{
		BorrowerNode * tmp;            //声明类型
		tmp=alloc_borrowernode();            //分配一个借阅者空间
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


//存储所有数据到数据文件
static int save_all_data()
{
	FILE *fp;
	BookNode *book_node=g_book_list;
	BorrowerNode *borrower_node=g_borrower_list;
	int node_count;
	fp=fopen(DATA_FILE_NAME,"wb");
	if(!fp)
		return -1;
	//先写入个数
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

//显示图书信息
static void display_book(Book *bo)
{
	assert(bo);
	printf("\t 书名：%s\n",bo->name);
	printf("\t 作者：&s\n",bo->author);
}

static void display_borrower(Borrower *br)      //显示读者信息
{
	assert(br);
	printf("\t 姓名：%s\n",br->name);
	printf("\t 性别：%s\n",br->sex);
	printf("\t 住址：%s\n",br->address);

}
int init_system(void) //初始化系统
{
	if(load_all_data())
		return -1;
	return 0;
}


//释放系统中的数据空间
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

//查找借阅者
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

//查找图书
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
//创建借阅者信息
int create_borrower()
{
	BorrowerNode *p=alloc_borrowernode();
	char ch;
	puts("\n\t<<注册借书者>>\n");
	printf("输入姓名：");
	gets(p->borrower.name);
	if(find_borrowernode(p->borrower.name))
	{
		puts("系统中有同名的借书者存在");
		goto lab_failed;
	}
	printf("输入性别：");
	gets(p->borrower.sex);
	printf("输入地址：");
	gets(p->borrower.address);
	printf("确定注册该借书者?(N/Y)");
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
	puts("\n注册借书者成功!");
	getch();
	return 0;
lab_failed:
	free_borrowernode(p);
	puts("\n注册借书者失败!");
	getch();
	return -1;
}


//建立图书信息
int create_book()
{
	BookNode *p=alloc_booknode();
	char ch;
	puts("\n\t<<注册图书>>\n");
	printf("输入书名：");
	gets(p->book.name);
	if(find_booknode(p->book.name))
	{
		puts("系统中有同名的图书存在");
		goto lab_failed;
	}
	printf("输入作者：");
	gets(p->book.author);
	printf("确定注销图书吗?(Y/N)");
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
	puts("\n注册图书成功!");
	getch();
	return 0;
lab_failed:
	free_booknode(p);
	puts("\n注册图书失败!");
	getch();
	return -1;
}

//删除借阅者
int remove_borrower()
{
	char name[_MAX_STRLEN];
	char ch;
	BorrowerNode *p;
	puts("\n\t<<注销借阅者>>\n");
	printf("输入姓名：");
	gets(name);
	p=find_borrowernode(name);
	if(!p)
	{
		puts("系统中不存在该借阅者");
		goto lab_failed;
	}
	printf("确定注销该借书者?(Y/N)");
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
	puts("\n注销借书者成功!");
	getch();
	return 0;
lab_failed:
	puts("\n注销借书者失败!");
	getch();
	return -1;
}

//删除图书
int remove_book()
{
	char name[_MAX_STRLEN];
	char ch;
	BookNode *p;
	puts("\n\t<<注销图书>>\n");
	printf("输入书名：");
	gets(name);
	p=find_booknode(name);

	if(!p)
	{
		puts("系统中不存在该图书");
		goto lab_failed;
	}
	printf("确定注销该图书?(Y/N)");
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
	puts("\n注销图书成功!");
	getch();
	return 0;
lab_failed:
	puts("\n注销图书失败!");
	getch();
	return -1;
	
}

//借书
int lend_book()
{
	char name[_MAX_STRLEN];
	char ch;
	BorrowerNode *br_node;
	BookNode *bo_node;
	puts("\n\t<<出借图书>>\n");
	printf("输入借书者姓名：");
	gets(name);
	br_node=find_borrowernode(name);
	if(!br_node)
	{
		puts("系统中不存在该借阅者");
		goto lab_failed;
	}
	if(br_node->borrower.lendnum>=_MAX_LENDBOOK)
	{
		printf("该借书者已借满%d本书\n",_MAX_LENDBOOK);
		goto lab_failed;
	}
	printf("输入要借图书书名：");
	gets(name);
	bo_node=find_booknode(name);
	if(!bo_node)
	{
		puts("系统中不存在该图书");
		goto lab_failed;
	}
	if(bo_node->book.borrowed)
	{
		printf("该图书已借给%s\n",bo_node->book.borrower);
		goto lab_failed;
	}
	printf("确定出借图书吗?(Y/N)");
	ch=getch();
	if(ch=='N'&&ch=='n')
		goto lab_failed;

	strcmp(br_node->borrower.book[br_node->borrower.lendnum],bo_node->book.name);
	strcmp(bo_node->book.borrower,br_node->borrower.name);
	br_node->borrower.lendnum++;
	bo_node->book.borrowed=1;
	puts("\n出借图书成功!");
	getch();
	return 0;
lab_failed:
	puts("\n出借图书失败!");
	getch();
	return -1;

}
//还书
int return_book()
{
	char name[_MAX_STRLEN];
	char ch;
	int i;
	BorrowerNode *br_node;
	BookNode *bo_node;
	puts("\n\t<<归还图书>>\n");
	printf("输入还书者姓名：");
	gets(name);
	br_node=find_borrowernode(name);
	if(!br_node)
	{
		puts("系统中不存在该还书者");
		goto lab_failed;
	}
	if(br_node->borrower.lendnum==0)
	{
		puts("该人没有借书");
		goto lab_failed;
	}
	printf("输入要还图书书名：");
	gets(name);
	bo_node=find_booknode(name);
	if(!bo_node)
	{
		puts("系统中不存在该图书");
		goto lab_failed;
	}
	if(!bo_node->book.borrowed)
	{
		printf("该图书没有被出借");
		goto lab_failed;
	}
	for(i=0;i<br_node->borrower.lendnum;i++)
	{
		if(0==strcmp(br_node->borrower.book[i],name))
			break;
	}
	if(i!=br_node->borrower.lendnum)
	{
		printf("该图书不是被%s所借，而是为%s所借\n",br_node->borrower.name,bo_node->book.borrower);
		goto lab_failed;
	}
	printf("确定归还图书吗?(Y/N)");
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
	puts("\n归还图书成功!");
	getch();
	return 0;
lab_failed:
	puts("\n归还图书失败!");
	getchar();
	return -1;
}

//显示借阅者信息
void display_borrower_info(Borrower *br)
{
	int  i;
	puts("==========================================");
	display_borrower(br);
	if(br->lendnum)
	{
		puts("&&&&&&&&&&&借书信息&&&&&&&&&&&&&");
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

//图书信息
void display_book_info(Book *bo)\
{
	puts("===================================");
	display_book(bo);
	if(bo->borrowed)
		printf("\t该书被%s外借\n",bo->borrower);
}

//查找借书者
int lookup_borrower()
{
	BorrowerNode *p;
	char name[_MAX_STRLEN];
	puts("\n\t<<查找借书者>>");
	printf("输入姓名：");
	gets(name);
	if(!(p=find_borrowernode(name)))
	{
		puts("系统中不存在该借阅者");
		return -1;
	}
	display_borrower_info(&p->borrower);
	getch();
	return 0;
}
//查找图书
int lookup_book()
{
	BookNode *p;
	char name[_MAX_STRLEN];
	puts("\n\t<<查找图书>>");
	printf("输入姓名：");
	gets(name);
	if(!(p=find_booknode(name)))
	{
		puts("系统中不存在该图书");
		return -1;
	}
	display_book_info(&p->book);
	getch();
	return 0;
}
//浏览所有借阅者
int browse_borrower()
{
	BorrowerNode *borrower_node=g_borrower_list;
	char ch;
	while(borrower_node)
	{
		display_borrower_info(&borrower_node->borrower);
		puts("按任意键显示下一个借书者信息，或按Q终止浏览........");
		ch=getch();
		if(ch=='q'&&ch=='Q')
			return 0;
		borrower_node=borrower_node->next;
	}
	puts("全部借书者信息浏览结束");
	getch();
	return 0;
}
//浏览全部图书
int browse_book()
{
	BookNode *book_node=g_book_list;
	char ch;
	while(book_node)
	{
		display_book_info(&book_node->book);
		puts("按任意键显示下一本图书信息，或按Q终止浏览........");
		ch=getch();
		if(ch=='q'&&ch=='Q')
			return 0;
		book_node=book_node->next;
	}
	puts("全部图书信息浏览结束");
	getch();
	return 0;
}