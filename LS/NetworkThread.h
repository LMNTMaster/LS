#pragma once
#include "NetworkThreadBase.h"

class LMNT_DLL CNetworkThread : public CNetworkThreadBase
{
	
public:
	CNetworkThread();
	~CNetworkThread();


private:
	HANDLE m_hCompletionPort;


public:
	void SetCompletionPort(HANDLE hCP);

public:
	virtual void RunThread() override;
};

