// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <winsock.h>
#include <Windows.h>
#include "reply.h"


#pragma comment(lib, "ws2_32.lib")

#define BUFF_SIZE 256
#define MAX_CLIENTS 5

//多线程来处理每个客户端的聊天请求
HANDLE threads[MAX_CLIENTS];
HANDLE hMutex; //创建互斥对象
int threadCount = 0;


//多线程执行函数
DWORD WINAPI ProcessClientRequests(LPVOID lpParam)
{
	int static count = 1;
	printf("第%d次执行多线程函数\n", count++);
	//获取传进来的客户端的SOCKET
	SOCKET* clieSock = (SOCKET *)lpParam;

	//定义用来接收和发送的字符串数组
	char recvBuff[BUFF_SIZE] = {0};
	char sendBuff[BUFF_SIZE] = {0};

	//循环接收客户端的发来的消息
	while(1)
	{
		//接收客户端发来的消息
		int recvLen = recv(*clieSock, recvBuff, BUFF_SIZE, 0);
		//判断客户端是否成功发来消息
		if(recvLen > 0)
		{
			printf("IP地址为%d的客户端发来：%s\n", *clieSock, recvBuff);
			//智能回复消息
			strcpy_s(sendBuff, BUFF_SIZE, reply(recvBuff));
			int sendLen = send(*clieSock, sendBuff, strlen(sendBuff), 0);
			if(sendLen > 0)
			{
				printf("智能回复成功\n");
				printf("我说：%s\n", sendBuff);
			}
		}
		//将用来接收和发送的字符串数组的清空
		memset(&recvBuff, 0, BUFF_SIZE);
		memset(&sendBuff, 0, BUFF_SIZE);
	}
	ReleaseMutex(hMutex);
	//对话完毕后关闭客户端的socket
	closesocket(*clieSock);
	return 0;
}



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

	hMutex = CreateMutex(NULL, FALSE, NULL); //创建互斥对象

	//判断是否超过了规定的客户端连接数
	while (1)
	{
		if (threadCount < MAX_CLIENTS)
		{
			//接受客户端的连接
			SOCKADDR clieAddr;
			int addrLen = sizeof(SOCKADDR);
			SOCKET clieSock = accept(servSock, (SOCKADDR*)&clieAddr, &addrLen);

			//判断客户端连接的套接字是否有效
			if (clieSock == INVALID_SOCKET)
			{
				printf("clieSock is invalid");
				getchar();
				return -1;
			}
			threads[threadCount++] = CreateThread(NULL, NULL, ProcessClientRequests, (LPVOID*)&clieSock, 0, NULL);
			WaitForSingleObject(hMutex, INFINITE);
			WaitForMultipleObjects(MAX_CLIENTS, threads, true, INFINITE);
		}
	}
	CloseHandle(threads);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

