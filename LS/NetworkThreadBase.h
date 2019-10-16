#pragma once
#include "Core.h"
class LMNT_DLL CNetworkThreadBase
{
public:
	CNetworkThreadBase();
	~CNetworkThreadBase();


private:
	HANDLE m_hThread;
	HANDLE m_hEvent;


public:
	void CreateThread();
	void WaitEvent();

	virtual void RunThread() = 0;

public:
	static unsigned int CALLBACK ThreadFunction(void* p);
};

