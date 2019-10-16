#pragma once
#include "Core.h"

class LMNT_DLL CNetworkPacketHub
{
	friend class CNetworkManager;
	friend class CNetworkThread;

public:
	CNetworkPacketHub();
	virtual ~CNetworkPacketHub();

protected:
	virtual void Read(class CNetworkSession* pSession, class CPacket* pPacket);
};

