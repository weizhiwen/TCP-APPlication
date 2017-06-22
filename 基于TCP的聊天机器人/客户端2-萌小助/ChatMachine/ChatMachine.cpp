// ChatMachine.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <WinSock.h>

#define BUFF_SIZE 256
#pragma comment(lib, "ws2_32.lib")

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
	//定义用于接收和发送的字符串数组
	char sendBuff[BUFF_SIZE] = { 0 };
	char recvBuff[BUFF_SIZE] = { 0 };

	//发送和接收消息
	int sendLen = 0;
	int recvLen = 0;
	while (1)
	{
		printf("萌小助：");
		scanf_s("%s", sendBuff, BUFF_SIZE);
		sendLen = send(clieSock, sendBuff, strlen(sendBuff), 0);
		if (sendLen > 0)
		{
			printf("发送成功了！\n");
			recvLen = recv(clieSock, recvBuff, BUFF_SIZE, 0);
			if (recvLen > 0)
			{
				printf("接收到了服务器端的回复\n");
				printf("聊天机器人：%s\n", recvBuff);
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
	return 0;
}


