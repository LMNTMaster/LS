#include "NetworkThreadBase.h"



CNetworkThreadBase::CNetworkThreadBase() : 
	m_hThread(NULL), m_hEvent(NULL)
{
}


CNetworkThreadBase::~CNetworkThreadBase()
{
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

void CNetworkThreadBase::CreateThread()
{
	m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = (HANDLE)_beginthreadex(0, 0, CNetworkThreadBase::ThreadFunction, this, 0, 0);
	SetEvent(m_hEvent);
}

void CNetworkThreadBase::WaitEvent()
{
	WaitForSingleObject(m_hEvent, INFINITE);
}

unsigned int CNetworkThreadBase::ThreadFunction(void * p)
{
	CNetworkThreadBase* pThread = (CNetworkThreadBase*)p;

	pThread->WaitEvent();

	pThread->RunThread();

	return 0;
}
