// server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <winsock.h>
#include "TellBook.h"

#pragma comment(lib, "ws2_32.lib") //动态加载链接库

#define BUFF_SIZE 256

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

	//创建服务器端的socket
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);

	//套接字绑定IP信息
	sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(SOCKADDR));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(4567);
	bind(servSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR));

	//进入监听状态
	int error = listen(servSock, 20);
	if (error != 0)
	{
		printf("服务器监听失败，启动失败\n");
		getchar();
		return -1;
	}
	printf("服务器已启动，等待客户端连接\n");

	//得到客户端连接的socket
	SOCKADDR clieAddr;
	int addrLen = sizeof(SOCKADDR);
	SOCKET clieSock = accept(servSock, (SOCKADDR*)&clieAddr, &addrLen);

	//定义用于接收和发送的字符串数组
	char recvBuff[BUFF_SIZE] = { 0 };
	char sendBuff[BUFF_SIZE] = { 0 };
	int recvLen = 0;
	int sendLen = 0;

	//服务器端接收和发送消息
	while (1)
	{
		recvLen = recv(clieSock, recvBuff, BUFF_SIZE, 0);
		//判断接收是否成功
		if (recvLen > 0)
		{
			printf("接收到客户端发来的消息:%s\n", recvBuff);
			//下面根据客户端的请求来进行相关的操作
			if (strcmp(recvBuff, "1") == 0)
			{
				printf("*******显示通讯录*******\n");
				printTellBook(); //显示通讯录
			}
			if (strcmp(recvBuff, "2") == 0)
			{
				strcpy_s(sendBuff, BUFF_SIZE, "请输入联系人的姓名，年龄，性别， 单位， 手机号， ip\n");
				if (send(clieSock, sendBuff, strlen(sendBuff), 0) > 0)
				{
					memset(recvBuff, 0, BUFF_SIZE);
					//添加联系人，需要继续接收一条来自客户端的联系人具体信息的消息
					if (recv(clieSock, recvBuff, BUFF_SIZE, 0) > 0)
					{
						printf("*******添加联系人*******\n");
						addTellPeople(recvBuff);
					}
				}
			}
			if (strcmp(recvBuff, "3") == 0)
			{
				strcpy_s(sendBuff, BUFF_SIZE, "请输入要删除的联系人的姓名：");
				if (send(clieSock, sendBuff, strlen(sendBuff), 0) > 0)
				{
					memset(recvBuff, 0, BUFF_SIZE);
					//删除联系人，需要继续接收一条来自客户端的联系人的姓名
					if (recv(clieSock, recvBuff, BUFF_SIZE, 0) > 0)
					{
						printf("删除联系人中······\n");
						deleteTellPeople(recvBuff);
					}
				}
			}
			memset(sendBuff, 0, BUFF_SIZE);
			strcpy_s(sendBuff, BUFF_SIZE, "服务器端已处理请求\n"); //发送的内容这里写死了
			sendLen = send(clieSock, sendBuff, strlen(sendBuff), 0);
			//判断发送是否成功
			if (sendLen > 0)
			{
				printf("发送成功\n");
			}
		}
		//重置发送和接收字符串的内容，清零
		memset(&recvBuff, 0, BUFF_SIZE);
		memset(&sendBuff, 0, BUFF_SIZE);
	}
	closesocket(clieSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

