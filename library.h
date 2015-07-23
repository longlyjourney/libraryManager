#ifndef _LIBRARY_H_
#define  _LIBRARY_H_

#define _MAX_STRLEN 80
#define _MAX_LENDBOOK 5

#define DATA_FILE_NAME "library.dat"    //���������ļ�

typedef struct _tagBorrower          //��������߽������
{
	char name[_MAX_STRLEN];           //����
	char sex[_MAX_STRLEN];            //�Ա�
	char address[_MAX_STRLEN];        //��ַ
	int lendnum;                       //���ı���
	char book[_MAX_LENDBOOK][_MAX_STRLEN];     //�����ı�������������
}Borrower;


typedef  struct _tabBook
{
	char name[_MAX_STRLEN];    //��������
	char  author[_MAX_STRLEN];         //����
	int   borrowed;            //����״̬
	char  borrower[_MAX_STRLEN];
}Book;

//����ͼ��˫������
typedef struct _tagBookNode
{
	Book book;
	struct  _tagBookNode *prev;
	struct  _tagBookNode *next;
}BookNode;

//����������˫������
typedef struct _tagBorrowerNode
{
	Borrower borrower;
	struct _tagBorrowerNode  *prev;
	struct _tagBorrowerNode  *next;
}BorrowerNode;

//�ⲿ��������
static BookNode  * g_book_list=0;
static BorrowerNode * g_borrower_list=0;

//����ԭ������
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