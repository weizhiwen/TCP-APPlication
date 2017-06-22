#pragma once

#include <iostream>
#define BUFF_SIZE 256

//定义联系人结构体
typedef struct TELLPEOPLE
{
	char name[15]; //姓名
	char age[3]; //年龄
	char sex[5]; //性别
	char unit[30]; //单位
	char tellnum[11]; //电话号码
	char ip[15]; //ip地址
}TELLPEOPLE;

//显示通许录文件中的信息
void printTellBook()
{
	FILE *fp;
	char line[BUFF_SIZE] = { 0 };
	//只读方式打开文件
	if (fopen_s(&fp, "tellbook.txt", "rt") == 0)
	{
		//循环读取每行的内容
		while (fgets(line, BUFF_SIZE, fp) != NULL)
		{
			printf("%s", line);
		}
		memset(&line, 0, BUFF_SIZE);
	}
	else
	{
		printf("打开电话本文件失败");
	}
	fclose(fp);
}

//添加联系人
void addTellPeople(char* recvBuff)
{
	FILE *fp;
	char line[BUFF_SIZE] = { 0 };
	//追加内容方式读写文件
	if (fopen_s(&fp, "tellbook.txt", "a+") == 0)
	{
		if (fputs(recvBuff, fp) != EOF)
		{
			printf("添加联系人成功！");
		}
		else
		{
			printf("添加联系人失败！");
		}
	}
	else
	{
		printf("打开电话本文件失败");
	}
	fclose(fp);
}

//删除联系人
void deleteTellPeople(char* recvBuff)
{
	//根据联系人的姓名删除文件的一行内容，思想是把除这一行外的所有数据复制到新建的文件，再删除原文件，最后重命名文件
	FILE *fp, *ftemp; //前一个文件指针指向原文件，后一个指针指向新的文件
	//创建一个可读写的文本文件
	if (fopen_s(&ftemp, "temp.txt", "w") == 0)
	{
		printf("临时文件创建成功！\n");
		fclose(ftemp);
	}
	else
	{
		printf("临时文件创建失败！\n");
		getchar();
		exit(-1);
	}
	//改变临时文件的读写方式，可以在文件末尾添加数据
	if (fopen_s(&ftemp, "temp.txt", "at+") == 0)
	{
		printf("改变文件的读写方式成功！\n");
	}
	else
	{
		printf("改变文件的读写方式失败！\n");
		getchar();
		exit(-1);
	}
	char line[BUFF_SIZE] = { 0 };
	//可读可写方式打开文件
	if (fopen_s(&fp, "tellbook.txt", "r") == 0)
	{
		//特殊处理文件第一行的内容
		fgets(line, BUFF_SIZE, fp); //从原文件读一行数据，此时文件指针fp会移动到下一行的开头
		//FILE *fpp = fp;
		strcat_s(line, BUFF_SIZE, "\n");
		fputs(line, ftemp); //添加到新建文件中
		TELLPEOPLE tellPeople = { "", "", "", "", "", "" };
		//删除联系人时要把从文件中读出来每行联系人的信息格式化处理一下
		bool isFind = false;
		//定义制表符和换行符
		char ct = '\t';
		char cn = '\n';
		//格式化读取文件中的内容
		while (fscanf_s(fp, "%s\t%s\t%s\t%s\t%s\t%s\n", tellPeople.name, 15, tellPeople.age, 3, tellPeople.sex, 5, tellPeople.unit, 30, tellPeople.tellnum, 11, tellPeople.ip, 15) != EOF)
		{
			if (strcmp(tellPeople.name, recvBuff) == 0)
			{
				printf("找到了要删除的联系人\n");
				isFind = true;
			}
			else
			{
				fflush(stdin);
				//格式化写入新的文件
				fprintf_s(ftemp, "%s%c%s%c%s%c%s%c%s%c%s\n", tellPeople.name, ct, tellPeople.age, ct, tellPeople.sex, ct, tellPeople.unit, ct, tellPeople.tellnum, ct, tellPeople.ip, cn);
				tellPeople = {"", "", "", "", "", ""};
			}
		}
		if (isFind == false)
			printf("没有找到要删除的联系人\n");
		memset(&line, 0, BUFF_SIZE);
	}
	else
	{
		printf("打开电话本文件失败");
	}
	//关闭文件指针，删除原文件，重命名临时文件
	fclose(ftemp);
	fclose(fp);
	remove("tellbook.txt");
	rename("temp.txt", "tellbook.txt");
}

