#include <Windows.h>
#include <cstdio>
#include <conio.h>
#include <iostream>

int					g_nValue = 0;
CRITICAL_SECTION	g_csValue;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	while (true)
	{
		for (size_t i = 0; i < 1000000; i++)
		{
			EnterCriticalSection(&g_csValue);
			g_nValue += 1;
			LeaveCriticalSection(&g_csValue);
		}
		break;
	}
	return 0;
}

int main()
{
	HANDLE hThread;
	DWORD dwThreadID;
	InitializeCriticalSection(&g_csValue);

	hThread = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadProc,
		NULL,
		0,
		&dwThreadID);

	while (true)
	{
		for (size_t i = 0; i < 1000000; i++)
		{
			EnterCriticalSection(&g_csValue);
			g_nValue -= 1;
			LeaveCriticalSection(&g_csValue);
		}
		break;
	}

	Sleep(2000);
	std::cout << g_nValue << std::endl;

	DeleteCriticalSection(&g_csValue);

	return 0;
}