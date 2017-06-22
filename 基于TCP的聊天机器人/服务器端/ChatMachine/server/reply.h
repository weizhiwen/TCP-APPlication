#pragma once
#include <iostream>
#define BUFF_SIZE 256

char oddLine[BUFF_SIZE]; //存放文件奇数行内容
char evenLine[BUFF_SIZE]; //存放文件偶数行内容

char* reply(char *recvBuff)
{
	strcat_s(recvBuff, BUFF_SIZE - strlen(recvBuff), "\n");
	FILE *fp;
	if (fopen_s(&fp, "language.txt", "rt") == 0)
	{
		//循环读取每行内容
		while (!feof(fp))
		{
			fgets(oddLine, BUFF_SIZE, fp); //读取奇数行内容
			fgets(evenLine, BUFF_SIZE, fp); //读取偶数行内容
			//比较奇数行内容，判断是否找到了智能回复的内容
			if (strcmp(oddLine, recvBuff) == 0)
			{
				return evenLine; //找到了就返回偶数行内容
			}
			memset(&oddLine, 0, BUFF_SIZE + 1);
			memset(&oddLine, 0, BUFF_SIZE + 1);
		}
	}
	else
	{
		printf("智能回复失败");
	}
	fclose(fp);
	return "我不知道说什么了";
}