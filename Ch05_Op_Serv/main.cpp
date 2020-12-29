#include <iostream>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>

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

int calculation(int opnum, int* opnds, char op)
{
	int result = opnds[0];

	switch (op)
	{
	case '+':
		for (size_t i = 1; i < opnum; i++)
		{
			result += opnds[i];
		}
		break;

	case '-':
		for (size_t i = 1; i < opnum; i++)
		{
			result -= opnds[i];
		}
		break;

	case '*':
		for (size_t i = 1; i < opnum; i++)
		{
			result *= opnds[i];
		}
		break;
	}

	return result;
}

int main(int argc, char** argv)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char opinfo[BUF_SIZE];
	int result, opndCnt;
	int recvCnt, recvLen;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSize;

	if (argc != 2)
	{
		std::cout << "Usage : " << argv[0] << "<PORT>" << std::endl;
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
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind error");
	}
	if (listen(hServSock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen error");
	}

	clntAdrSize = sizeof(clntAdr);

	for (size_t i = 0; i < 5; i++)
	{
		opndCnt = 0;
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSize);
		recv(hClntSock, reinterpret_cast<char*>(&opndCnt), 1, 0);

		recvLen = 0;
		while ((opndCnt * OP_SIZE + 1) > recvLen)
		{
			recvCnt = recv(hClntSock, &opinfo[recvLen], BUF_SIZE - 1, 0);
			recvLen += recvCnt;
		}

		result = calculation(opndCnt, reinterpret_cast<int*>(opinfo), opinfo[recvLen - 1]);
		send(hClntSock, reinterpret_cast<char*>(&result), sizeof(result), 0);
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();

	return 0;
}