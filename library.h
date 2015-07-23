#ifndef _LIBRARY_H_
#define  _LIBRARY_H_

#define _MAX_STRLEN 80
#define _MAX_LENDBOOK 5

#define DATA_FILE_NAME "library.dat"    //定义数据文件

typedef struct _tagBorrower          //定义借阅者结点类型
{
	char name[_MAX_STRLEN];           //名字
	char sex[_MAX_STRLEN];            //性别
	char address[_MAX_STRLEN];        //地址
	int lendnum;                       //借阅本书
	char book[_MAX_LENDBOOK][_MAX_STRLEN];     //最多借阅本数，书名长度
}Borrower;


typedef  struct _tabBook
{
	char name[_MAX_STRLEN];    //定义书名
	char  author[_MAX_STRLEN];         //作者
	int   borrowed;            //借阅状态
	char  borrower[_MAX_STRLEN];
}Book;

//声明图书双向链表
typedef struct _tagBookNode
{
	Book book;
	struct  _tagBookNode *prev;
	struct  _tagBookNode *next;
}BookNode;

//声明借阅者双向链表
typedef struct _tagBorrowerNode
{
	Borrower borrower;
	struct _tagBorrowerNode  *prev;
	struct _tagBorrowerNode  *next;
}BorrowerNode;

//外部声明定义
static BookNode  * g_book_list=0;
static BorrowerNode * g_borrower_list=0;

//函数原型声明
int init_system();
void release_system();
int create_borrower();
int create_book();
int remove_borrower();
int remove_book();
int lend_book();
int return_book();
int lookup_borrower();
int lookup_book();
int browse_book();
int browse_borrower();
#endif