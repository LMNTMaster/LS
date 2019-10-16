#pragma once
#include "Core.h"

class CNetworkClient
{
public:
	CNetworkClient();
	virtual ~CNetworkClient();

private:
	std::unordered_map<std::string, void(*)(SOCKET, class CNetworkSession*)> m_mapEventFunction;
	
public:
	void AddEvent(const std::string& strEventKey, void(*fp)(SOCKET, class CNetworkSession*));
	void CallEvent(const std::string& strEventKey, SOCKET hSocket, class CNetworkSession* pSession);
	void RemoveEvent(const std::string& strEventKey);
public:
	virtual void Connect();
	virtual void Enter();
	virtual void Leave();
	virtual void Disconnect();
};

