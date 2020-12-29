#include <iostream>
#include <stdlib.h>
#include <WinSock2.h>
#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

INT _tmain(INT argc, TCHAR argv[])
{
	WSADATA wsaData;		// 윈도우 소켓 정보
	SOCKET hServSock, hClnSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello world!";
	if (argc != 2)
	{
		_tprintf(TEXT("Usage : %s <port>\n"), argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup error");
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}