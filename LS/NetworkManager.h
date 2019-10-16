#pragma once
#include "Core.h"
#include "NetworkPacketHub.h"


class LMNT_DLL CNetworkManager
{
	friend class CNetworkThread;

private:
	CNetworkManager();
	~CNetworkManager();

private:
	static CNetworkManager* m_pInstance;
public:
	static CNetworkManager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CNetworkManager;
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (nullptr != m_pInstance)
			delete m_pInstance;

		m_pInstance = null;
	}


private:
	static std::unordered_map<int, std::string> m_mapDebugErrorDescription;
public:
	static void PrintErrorDescription();


private:
	bool m_bIsServer;
	std::string m_strAddress;
	SHORT m_iPort;
	CNetworkPacketHub* m_pPacketHub;
public:
	template<typename DecoderClass = CNetworkPacketHub>
	bool SetDecoderClass()
	{		
		if (std::is_convertible<DecoderClass, CNetworkPacketHub>())
		{
			m_pPacketHub = new DecoderClass;
			return true;
		}
		else
		{
			m_pPacketHub = new CNetworkPacketHub;
			return false;
		}
	}

/*CLIENT*/
private:
	//std::unordered_map<std::string, void(*)()> m_mapEventFunction;
	SOCKET m_hSocket;
	HANDLE m_hThread;
	CPacket* m_pReadPacket;
	CPacket* m_pWritePacket;
	static bool m_bLoop;
	static unsigned int CALLBACK ClientLoopProcedure(void* pThis);
	SOCKET GetSocket() const;
public:
	CPacket* GetReadPacket();
	CPacket* GetWritePacket();
	void SendToServer();
/*CLIENT*/


//public:
//	void AddEvent(const std::string& strEventKey, void(*fp)());
//	void CallEvent(const std::string& strEventKey);
//	void RemoveEvent(const std::string& strEventKey);

private:
	HANDLE m_hCompletionPort;
	class CNetworkSession* m_pListen;
	std::unordered_map<SOCKET, CNetworkSession*> m_mapClientSession;
	std::vector<class CNetworkThreadBase*> m_vecThread;

public:
	bool Initialize(bool isServer = false, std::string targetServerAddress = "");
	void Update();

public:
	void AddClient(SOCKET hSocket, CNetworkSession* pSession);
	void RemoveClient(SOCKET hSocket);
	const CNetworkSession* GetClientSession(SOCKET hSocket) const;
	
	SHORT GetPort()const;
	int32_t GetPingTime(class CNetworkSession* user)const;
	int32_t GetReturnTripTime(class CNetworkSession* user)const;
	int32_t GetPingTime(SOCKET user)const;
	int32_t GetReturnTripTime(SOCKET user)const;
	
	void ConnectIOCP(class CNetworkSession* pSession)const;

};

