// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <WinSock.h>
#include "TellPeople.h"

#define BUFF_SIZE 256
#pragma comment(lib, "ws2_32.lib")
void showmenu();

int _tmain(int argc, _TCHAR* argv[])
{
	//定义在windows下的socket编程
	WORD wsaWord = MAKEWORD(2, 2);
	WSADATA wsaData;
	int wsaErr = WSAStartup(wsaWord, &wsaData);
	if (wsaErr != 0)
	{
		printf("WSAStartup failed with error: %d\n", wsaErr);
		getchar();
		return -1;
	}

	//创建客户端的socket
	SOCKET clieSock = socket(AF_INET, SOCK_STREAM, 0);

	//设置服务器端的相关信息
	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(4567);

	//连接服务器端
	int err = connect(clieSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));
	if (err != 0)
	{
		printf("连接服务器端失败\n");
		getchar();
		return -1;
	}
	printf("连接服务器成功\n");

	showmenu(); //显示菜单

	//定义用于接收和发送的字符串数组
	char sendBuff[BUFF_SIZE] = { 0 };
	char recvBuff[BUFF_SIZE] = { 0 };
	//发送和接收消息
	int sendLen = 0;
	int recvLen = 0;
	while (1)
	{
		printf("请输入你想执行的功能序号：");
		scanf_s("%s", sendBuff, BUFF_SIZE);
		//判断是否要退出，本条信息不发送给服务器端，直接在客户端判断退出
		if (strcmp(sendBuff, "4") == 0)
		{
			printf("欢迎下次使用\n");
			break;
		}
		sendLen = send(clieSock, sendBuff, strlen(sendBuff), 0);
		//判断是否发送成功
		if (sendLen > 0)
		{
			printf("发送成功了！\n");
			//如果客户的选项是2，则需要客户端多发一次消息，服务器端多收一次消息
			if (strcmp(sendBuff, "2") == 0)
			{
				TELLPEOPLE tellPeople;
				char ct = '\t';
				char cn = '\n';
				if (recv(clieSock, recvBuff, BUFF_SIZE, 0) > 0)
				{
					printf("%s", recvBuff);
					scanf_s("%s", tellPeople.name, 15);
					scanf_s("%s", tellPeople.age, 3);
					scanf_s("%s", tellPeople.sex, 5);
					scanf_s("%s", tellPeople.unit, 30);
					scanf_s("%s", tellPeople.tellnum, 11);
					scanf_s("%s", tellPeople.ip, 15);
					//scanf_s("%s%s%s%s%s%s", tellPeople.name, 15,  &tellPeople.age, 3, tellPeople.sex, 5, tellPeople.unit, 30, tellPeople.tellnum, 11, tellPeople.ip, 15);
				}
				memset(&sendBuff, 0, BUFF_SIZE);
				sprintf_s(sendBuff, "%s%c%s%c%s%c%s%c%s%c%s%c", tellPeople.name, ct, tellPeople.age, ct, tellPeople.sex, ct, tellPeople.unit, ct, tellPeople.tellnum, ct, tellPeople.ip, cn);
				printf("%s", sendBuff);
				if (send(clieSock, sendBuff, strlen(sendBuff), 0) > 0)
				{
					printf("发送成功！\n");
				}
			}
			//如果客户的选项是3，则需要客户端多发一下要删除的联系人的姓名，服务器端多收一次消息
			if (strcmp(sendBuff, "3") == 0)
			{
				if (recv(clieSock, recvBuff, BUFF_SIZE, 0) > 0)
				{
					printf("%s", recvBuff);
					char name[15] = { 0 };
					scanf_s("%s", name, 15);
					strcpy_s(sendBuff, BUFF_SIZE, name);
				}
				if (send(clieSock, sendBuff, strlen(sendBuff), 0) > 0)
				{
					printf("发送成功！\n");
				}
			}
			memset(recvBuff, 0, BUFF_SIZE);
			recvLen = recv(clieSock, recvBuff, BUFF_SIZE, 0);
			//判断是否接收成功
			if (recvLen > 0)
			{
				printf("%s\n", recvBuff);
			}
			else
			{
				printf("没有接收到服务器端的回复\n");
			}
		}
		memset(&sendBuff, 0, BUFF_SIZE);
		memset(&recvBuff, 0, BUFF_SIZE);
	}
	closesocket(clieSock);
	WSACleanup();
	system("pause");
	return 0;
}

void showmenu()
{
	printf("************欢迎使用基于TCP的通讯录************\n");
	printf("		1.显示联系人信息\n");
	printf("		2.添加联系人\n");
	printf("		3.删除联系人\n");
	printf("		4.退出\n");
	printf("***********************************************\n");
}