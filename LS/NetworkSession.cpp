#include "NetworkSession.h"
#include "NetworkManager.h"



CNetworkSession::CNetworkSession()
{
	m_hSocket = NULL;
	memset(&m_tAddress, 0, sizeof(m_tAddress));
	memset(&m_tReadOverlapped, 0, sizeof(m_tReadOverlapped));
	memset(&m_tWriteOverlapped, 0, sizeof(m_tWriteOverlapped));
	m_dwReadTransfer = 0;
	m_dwWriteTransfer = 0;
	m_tReadOverlapped.m_bRead = true;
	m_tWriteOverlapped.m_bRead = false;


}


CNetworkSession::~CNetworkSession()
{
	if (m_hSocket)
	{
		OnDisconnect();
		CNetworkManager::GetInstance()->RemoveClient(m_hSocket);
		shutdown(m_hSocket, SD_BOTH);
	}
}

SOCKET CNetworkSession::GetSocket() const
{
	return m_hSocket;
}

CPacket * CNetworkSession::GetReadPacket()
{
	return &m_tReadPacket;
}

CPacket * CNetworkSession::GetWritePacket()
{
	return &m_tWritePacket;
}

void CNetworkSession::Read()
{
	m_tReadPacket.ClearPacketData();
	m_tReadOverlapped.m_tBuffer.buf = (char*)&m_tReadPacket;
	m_tReadOverlapped.m_tBuffer.len = PACKET_SIZE + 8;
	DWORD dwFlag = 0;
	WSARecv(m_hSocket, &m_tReadOverlapped.m_tBuffer, 1, &m_dwReadTransfer, &dwFlag, &m_tReadOverlapped.m_tOverlapped, NULL);
}


void CNetworkSession::Write(uint32_t iHeader, uint32_t iSize, const char * pData)
{
	m_tWritePacket.ClearPacketData();
	m_tWritePacket.SetPacketHeader(iHeader);
	m_tWritePacket.PushData(pData, iSize);

	m_tWriteOverlapped.m_tBuffer.buf = (char*)&m_tWritePacket;
	m_tWriteOverlapped.m_tBuffer.len = m_tWritePacket.GetSize() + 8;

	DWORD dwFlag = 0;
	WSASend(m_hSocket, &m_tWriteOverlapped.m_tBuffer, 1, &m_dwWriteTransfer, dwFlag, &m_tWriteOverlapped.m_tOverlapped, NULL);
}

void CNetworkSession::Write(const char * strMessage)
{
	Write(PACKET_STRING_MESSAGE, strlen(strMessage), strMessage);
}

void CNetworkSession::Write(const std::string & strMessage)
{
	Write(PACKET_STRING_MESSAGE, (uint32_t)strMessage.length(), strMessage.c_str());
}

void CNetworkSession::CreateListen(SHORT sPort)
{
	m_hSocket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	m_tAddress = {};
	m_tAddress.sin_family = PF_INET;
	m_tAddress.sin_port = htons(sPort);
	m_tAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(m_hSocket, (SOCKADDR*)&m_tAddress, sizeof(m_tAddress));
	listen(m_hSocket, SOMAXCONN);
}

CNetworkSession * CNetworkSession::Accept()
{
	SOCKADDR_IN tAddress = {};
	int iSize = sizeof(tAddress);
	SOCKET hSocket = accept(m_hSocket, (SOCKADDR*)&tAddress, &iSize);
	//accept로 연결 요청이 있는 애를 받아서, 소켓을 만들어준다.

	uint8_t ip[4];
	memcpy(ip, &tAddress.sin_addr, 4);	
	
	CNetworkSession* pSession = new CNetworkSession;
	pSession->m_hSocket = hSocket;
	memcpy(&pSession->m_tAddress, &tAddress, sizeof(tAddress));

	pSession->OnConnect();
	
	//매니저에서 관리할 수 있도록 해당 유저의 세션을 보관 하도록 함
	CNetworkManager::GetInstance()->AddClient(hSocket, pSession);
	
	
	//생성된 소켓을 CP 오브젝트에 연결을 해줘야 해요.
	CNetworkManager::GetInstance()->ConnectIOCP(pSession);
	
	//Read 시킨다.
	pSession->Read();
	
	return pSession;
}

//void CNetworkSession::AddEvent(const std::string & strEventKey, void(*fp)())
//{
//	std::unordered_map<std::string, void(*)()>::iterator iter = m_mapEventFunction.find(strEventKey);
//	if (iter == m_mapEventFunction.end())
//	{
//		m_mapEventFunction.insert(std::make_pair(strEventKey, fp));
//	}
//}
//
//void CNetworkSession::CallEvent(const std::string & strEventKey)
//{
//	std::unordered_map<std::string, void(*)()>::iterator iter = m_mapEventFunction.find(strEventKey);
//	if (iter != m_mapEventFunction.end())
//	{
//		iter->second();
//	}
//}
//
//void CNetworkSession::RemoveEvent(const std::string & strEventKey)
//{
//	std::unordered_map<std::string, void(*)()>::iterator iter = m_mapEventFunction.find(strEventKey);
//	if (iter != m_mapEventFunction.end())
//	{
//		m_mapEventFunction.erase(iter);
//	}
//}

void CNetworkSession::OnConnect()
{
	uint8_t ip[4];
	memcpy(ip, &m_tAddress.sin_addr, 4);
	std::cout << "Client Connected.		ID : " << m_hSocket << "	IP : " << (int)ip[0] << "." << (int)ip[1] << "." << (int)ip[2] << "." << (int)ip[3] << std::endl;
}
 
void CNetworkSession::OnEnter()
{
}

void CNetworkSession::OnLeave()
{
}

void CNetworkSession::OnDisconnect()
{
	uint8_t ip[4];
	memcpy(ip, &m_tAddress.sin_addr, 4);
	std::cout << "Client Disonnected.	ID : " << m_hSocket << "	IP : " << (int)ip[0] << "." << (int)ip[1] << "." << (int)ip[2] << "." << (int)ip[3] << std::endl;
}
