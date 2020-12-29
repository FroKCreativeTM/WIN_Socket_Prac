#include <Windows.h>
#include <cstdio>
#include <conio.h>
#include <iostream>

struct KCriticalSection : public CRITICAL_SECTION
{
	KCriticalSection() { InitializeCriticalSection(this); }
	~KCriticalSection() { DeleteCriticalSection(this); }
};

class KCriticalSectionLock
{
public:
	KCriticalSectionLock(CRITICAL_SECTION& cs) : m_pcs(&cs)
	{
		EnterCriticalSection(m_pcs);
	}

	~KCriticalSectionLock()
	{
		if (m_pcs != nullptr)
			LeaveCriticalSection(m_pcs);
	}
	explicit            operator bool() { return true; }
protected:
	CRITICAL_SECTION* m_pcs;
};

#define CSLOCK( cs_ )   if( KCriticalSectionLock lock = cs_ )

int						g_nValue = 0;
KCriticalSection		g_csValue;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	while (true)
	{
		for (size_t i = 0; i < 1000000; i++)
		{
			CSLOCK(g_csValue)
			{
				g_nValue += 1;
			}
		}
		break;
	}
	return 0;
}

int main()
{
	HANDLE hThread;
	DWORD dwThreadID;

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
			CSLOCK(g_csValue)
			{
				g_nValue -= 1;
			}
		}
		break;
	}

	Sleep(2000);
	std::cout << g_nValue << std::endl;

	return 0;
}