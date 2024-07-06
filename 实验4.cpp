#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<iostream>
using namespace std;
char str[128];
//采用C库函数实现文件备份
void C_copy()
{
	FILE* p1 = fopen("/home/cccp/file.txt", "r");//打开文件
	//如果打开失败
	if (p1 == NULL)
	{
		cout << "文件打开失败!" << endl;
		return;
	}
	else
	{
		//打开备份目标文件,若不存在则创建一个
		FILE* p2 = fopen("/home/cccp/copyfile_1.txt", "w+");
		if (p2 == NULL)//打开失败
		{
			cout << "文件打开失败!" << endl;
			fclose(p1);//关闭文件file.txt
			return;
		}
		else//成功
		{
			//在file中读取到str中，再copy到copyfile中
			int size;
			while (size = fread(str, sizeof(char), 128, p1))//fread返回一个int值
			{
				fwrite(str, sizeof(char), size, p2);//写入文件
			}
		}
		fclose(p1);//关闭文件
		fclose(p2);
		cout << "文件备份成功" << endl;
	}
	return;
}
//采用系统调用实现文件备份
void system_copy()
{
	int p1, p2;
	p1 = open("/home/cccp/file.txt", O_RDONLY);//打开文件
	if (p1==-1)//若文件打开失败
	{
		cout << "文件打开失败!" << endl;
		return;
	}
	else
	{
		p2 = open("/home/cccp/copyfile_2.txt", O_WRONLY|O_CREAT);//打开文件
		if (p2 == -1)
		{
			cout << "文件打开失败!" << endl;
			close(p1);//关闭文件
			return;
		}
		else
		{
			int size;
			while (size = read(p1, str, sizeof(str)))//读入
			{
				write(p2, str, sizeof(str));//写
			}
			close(p1);//关闭文件
			close(p2);
			cout << "文件备份完成" << endl;
		}
	}
	return;
}
int main()
{
	//C库函数
	C_copy();
	//系统调用函数
	system_copy();
	return 0;
}
