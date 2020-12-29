#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>

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
	SOCKET hServSock;
	char message[BUF_SIZE];
	int strLen;
	int clntAdrSize;

	SOCKADDR_IN servAdr, clntAdr;

	if (argc != 2)
	{
		std::cout << "Usage : " << argv[0] << "<PORT>" << std::endl;
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup error");
	}

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket error");
	}

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind error");
	}

	while (1)
	{
		clntAdrSize = sizeof(clntAdr);
		strLen = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAdr, &clntAdrSize);
		sendto(hServSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdr));
	}
	closesocket(hServSock);
	WSACleanup();

	return 0;
}