#include "NetworkClient.h"



CNetworkClient::CNetworkClient()
{
}

CNetworkClient::~CNetworkClient()
{
}

void CNetworkClient::AddEvent(const std::string & strEventKey, void(*fp)(SOCKET, CNetworkSession *))
{
	std::unordered_map<std::string, void(*)(SOCKET, CNetworkSession*)>::iterator iter = m_mapEventFunction.find(strEventKey);
	if (iter == m_mapEventFunction.end())
	{
		m_mapEventFunction.insert(std::make_pair(strEventKey, fp));
	}
}

void CNetworkClient::CallEvent(const std::string & strEventKey, SOCKET hSocket, CNetworkSession * pSession)
{
	std::unordered_map<std::string, void(*)(SOCKET, CNetworkSession*)>::iterator iter = m_mapEventFunction.find(strEventKey);
	if (iter != m_mapEventFunction.end())
	{
		iter->second(hSocket, pSession);
	}
}

void CNetworkClient::RemoveEvent(const std::string & strEventKey)
{
	std::unordered_map<std::string, void(*)(SOCKET, CNetworkSession*)>::iterator iter = m_mapEventFunction.find(strEventKey);
	if (iter != m_mapEventFunction.end())
	{
		m_mapEventFunction.erase(iter);
	}
}

void CNetworkClient::Connect()
{
}

void CNetworkClient::Enter()
{
}

void CNetworkClient::Leave()
{
}

void CNetworkClient::Disconnect()
{
}
