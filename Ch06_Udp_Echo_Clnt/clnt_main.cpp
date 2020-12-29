#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>

#pragma warning(disable:4996)

const int BUF_SIZE = 30;

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUF_SIZE];
	int strLen;

	SOCKADDR_IN servAdr;

	if (argc != 3)
	{
		std::cout << "Usage : " << argv[0] << "<IP> <PORT>" << std::endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup error");
	}

	hSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("socket error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	// connected UDP
	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		fputs("Insert message (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		send(hSocket, message, strlen(message), 0);
		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		printf("Message from server : %s", message);
	}
	closesocket(hSocket);
	WSACleanup();

	return 0;
}