#include <iostream>
#include <cstdlib>
#include <winsock2.h>

#pragma warning(disable:4996)

const int BUF_SIZE = 1024;
const int RLT_SIZE = 4;
const int OP_SIZE = 4;

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET hServSock;
	char opmsg[BUF_SIZE];
	int result, opndCnt;
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

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	if (connect(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect error");
	}
	else
	{
		fputs("Connected..................", stdout);
	}

	fputs("Operand count : ", stdout);
	scanf("%d", &opndCnt);
	opmsg[0] = (char)opndCnt;

	for (size_t i = 0; i < opndCnt; i++)
	{
		std::cout << "Operand : ";
		scanf("%d", (int*)&opmsg[i * OP_SIZE + 1]);
	}

	fgetc(stdin);
	fputs("Operator : ", stdout);
	scanf("%c", &opmsg[opndCnt * OP_SIZE + 1]);
	send(hServSock, opmsg, opndCnt * OP_SIZE + 2, 0);
	recv(hServSock, reinterpret_cast<char*>(&result), RLT_SIZE, 0);

	printf("Operation result : %d\n", result);
	closesocket(hServSock);
	WSACleanup();

	return 0;
}