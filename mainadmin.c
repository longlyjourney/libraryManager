#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "library.h"
#include "lirary.c"
//显示维护菜单
void display_maintenance_menu()
{
	system("cls");
	puts("\n\n\t\t<<维护子菜单(DEFEND MUNU)>>\n\n");
	puts("\t1 --注册用户(ENROLL USER)");
	puts("\t2 --注册图书(ENROLL BOOK)");
	puts("\t3 --注销用户(CANCEL USER)");
	puts("\t4 --注销图书(CANCEL BOOK)");
	puts("\t5 --版本信息(EDITION INFORMATION)");
	puts("\t0 --返回子菜单(RETURN MENU)");
}

//信息查找菜单
void display_info_munu()
{
	system("cls");
	puts("\n\n\t\t<<信息查找子菜单(FIND INFORMATION OF THE MEMU)>>\n\n");
	puts("\t1 --查找用户信息(FIND USER'S INFORMATION)");
	puts("\t2 --查找图书信息(FIND BOOK'S INFORMATION)");
	puts("\t3 --浏览用户信息(BROWSE USER'S INFORMATION)");
	puts("\t4 --浏览图书信息(BROWSE BOOK'S INFORMATION)");
	puts("\t0 --返回主菜单(RETURN MENU)");

}
//主菜单
void display_mainmenu()
{
	system("cls");
	puts("\n\t\t         开封大学图书管理系统                 \n\n");
	puts("\t\t\t         ====================\n\n");
	puts("\t             1 --借书(BORROW BOOK)");
	puts("\t             2 --还书(RETURN BOOK)");
	puts("\t             3 --管理(MANAGE)");
	puts("\t             4 --查找(FIND)");
	puts("\t             0 --退出(EXIT)");
	puts("\n");
	puts("\t     --请选择(CHOOSE):           ");
}
//程序结束
void display_end()
{
	system("cls");
	puts("\n\n\n\n\n\n\t谢谢使用，欢迎下次光临!\n");
}



//主程序
int main(void)
{
	int quit=0;
	char ch;
	
extern	init_system();
	do
	{
		display_mainmemu();
		ch=getch();
		switch(ch)
		{
			case '1':
				lend_book();
				break;
			case '2':
				return_book();
				break;
			case '3':
				{
					int sub_run=1;
					while(sub_run)
					{
						display_maintenance_menu();
						ch=getch();
					    switch(ch);
						{
							case 1:
								{	extern	create_borrower();
								break;}
							case 2:
								{	extern	create_book();
								break;}
							case 3:
								{	extern	remove_borrower();
								break;}
							case 4:
								{	extern	remove_book();
								break;}
							case 0:
								sub_run=0;
								break;
						}
					}
				}
				break;
			case '4':
				{
					int sub_run=1;

					while(sub_run)
					{
						display_info_menu();
						ch=getch();
						switch(ch)
						{
							case 1:
								{	extern	lookup_borrower();
								break;}
							case 2:
								{	extern	lookup_book();
								break;}
							case 3:
								{   extern	browse_borrower();
								break;}
							case 4:
								{	extern	browse_book();
								break;}
							case 0:
								sub_run=0;
								break;
						}
				    }
				}
				break;
			case '0':
				quit=1;
				break;
		}
	}while(!quit);
	release_system();}
	display_end();
	getch();
	return 0;
}