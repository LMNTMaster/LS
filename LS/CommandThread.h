#pragma once
#include "NetworkThreadBase.h"
class CCommandThread : public CNetworkThreadBase
{
public:
	CCommandThread();
	~CCommandThread();

protected:
	virtual void RunThread() = 0;
};

