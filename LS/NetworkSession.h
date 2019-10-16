#pragma once
#include "Core.h"
#include "Packet.h"

class LMNT_DLL CNetworkBuffer
{
public:
	OVERLAPPED m_tOverlapped;
	WSABUF		m_tBuffer;
	bool				m_bRead = false;
};

class LMNT_DLL CNetworkSession
{
private:
	friend class CNetworkManager;

public:
	CNetworkSession();
	~CNetworkSession();

private:
	SOCKET m_hSocket;
	SOCKADDR_IN m_tAddress;
	CNetworkBuffer m_tReadOverlapped;
	CNetworkBuffer m_tWriteOverlapped;
	DWORD	m_dwReadTransfer;
	DWORD m_dwWriteTransfer;

	CPacket	m_tReadPacket;
	CPacket m_tWritePacket;

public:
	SOCKET GetSocket() const;
	CPacket* GetReadPacket();
	CPacket* GetWritePacket();
	
	void Read();
	void Write(uint32_t iHeader, uint32_t iSize, const char* pData);
	void Write(const char* strMessage);
	void Write(const std::string& strMessage);

private:
	void CreateListen(SHORT sPort);
	CNetworkSession* Accept();


public:
	virtual void OnConnect();
	virtual void OnEnter();
	virtual void OnLeave();
	virtual void OnDisconnect();
};

