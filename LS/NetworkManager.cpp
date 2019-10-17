#include "NetworkManager.h"
#include "NetworkThread.h"
#include "NetworkSession.h"
#include "CommandThread.h"

CNetworkManager* CNetworkManager::m_pInstance = null;
bool CNetworkManager::m_bLoop = true;
std::unordered_map<int, std::string> CNetworkManager::m_mapDebugErrorDescription;

CNetworkManager::CNetworkManager()
{
#pragma region Debug String
	{
		m_mapDebugErrorDescription.insert(std::make_pair(6, "������ �̺�Ʈ ��ü �ڵ��� �߸� �Ǿ���."));
		m_mapDebugErrorDescription.insert(std::make_pair(8, "�޸𸮰� ������� �ʴ�."));
		m_mapDebugErrorDescription.insert(std::make_pair(87, "�ϳ� �̻��� �߸��� ���ڰ� ���ƴ�."));
		m_mapDebugErrorDescription.insert(std::make_pair(995, "overlapped(:12) ������ �ߴ� �Ǿ���."));
		m_mapDebugErrorDescription.insert(std::make_pair(996, "Overlapped ����� �̺�Ʈ�� ��ȣ ���°� �ƴϴ�."));
		m_mapDebugErrorDescription.insert(std::make_pair(997, "Overlapped ������ ���߿� �Ϸ�� ���̴�. ��ø ������ ���� �غ� �Ǿ�����, ��� �Ϸ���� �ʾ��� ���߻�"));
		m_mapDebugErrorDescription.insert(std::make_pair(10004, "WSACancelBlockingCall()�� ���� ���ȭ ȣ���� ���: Interrupted system call"));
		m_mapDebugErrorDescription.insert(std::make_pair(10009, "�߸��� ���� ����ڰ� ���Ǿ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10013, "��û�� �ּҰ� ��ε�ĳ��Ʈ �ּ��ε� setsockopt()�� SO_BROADCAST �� �������� �ʾҴ�."));
		m_mapDebugErrorDescription.insert(std::make_pair(10014, "�߸��� �ּҸ� �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10022, "���ε� ����. �̹� bind�� ���Ͽ� ���ε��ϰų� �ּ�ü�谡 �ϰ������� �ʾҴ�.\n �Ǵ� listen() ȣ�� ���� accept()�� ȣ��Ǿ��ų�, �ʱ�ȭ�� ��ϵ� Completion Routine�� NULL�� �ƴϴ�."));
		m_mapDebugErrorDescription.insert(std::make_pair(10024, "�ʹ� ���� ������ ��������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10035, "non overlapped ���� : �� ���� ���Ͽ� ���� ���� �����Ͱ� ����, overlapped(:12) ���� : �ʹ� ���� ��ø ����� �䱸�� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10036, "���ȭ �Լ��� ����Ǵ� ���� �������� ���� API�Լ��� ȣ��."));
		m_mapDebugErrorDescription.insert(std::make_pair(10037, "������� ���Ͽ��� ���� �������� �۾���. (connect�� �Ϸ���� ���� ���¿��� connect �Լ��� ȣ���..)"));
		m_mapDebugErrorDescription.insert(std::make_pair(10038, "�߸��� ���ϱ���ڸ� �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10039, "������ �ּҸ� ������� �ʾ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10040, "�ۼ��ſ� ���� �����Ͱ� ������ ũ�⸦ �ʰ��ؼ� ũ�⿡ �°� �߷���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10041, "���Ͽ� ���� �ʴ� ���������� �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10042, "�߸��� �ɼ��� �������. �������� �ʴ� �ɼ����� getsockopt�Լ��� ȣ���ϴ� ��."));
		m_mapDebugErrorDescription.insert(std::make_pair(10043, "���ӿ��� �������� �ʴ� �ּ�ü�踦 �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10044, "����Ÿ���� �������� �ʴ� �ּ�ü�踦 �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10045, "������ �������� �ʴ� ����� �������. listen()�Լ��� �����ͱ׷� ��� (SO_DGRAM)���� ȣ��"));
		m_mapDebugErrorDescription.insert(std::make_pair(10046, "�������� �ʴ� ���������� �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10047, "���ӿ��� �������� �ʴ� �ּ�ü�踦 �������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10048, "������ �ּҰ� �̹� ��� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10049, "����� �� ���� �ּ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10050, "��Ʈ��ũ ���� �ý��ۿ� ������ ����. ��Ʈ��ũ ���� �����."));
		m_mapDebugErrorDescription.insert(std::make_pair(10051, "��ũ��ũ�� ������ �� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10052, "��Ʈ��ũ �缳������ ������ ��������."));
		m_mapDebugErrorDescription.insert(std::make_pair(10053, "Ÿ�Ӿƿ� Ȥ�� ������ ���������� ���� ����Ʈ�������� ������ ������ ������."));
		m_mapDebugErrorDescription.insert(std::make_pair(10054, "������ ���� ȣ��Ʈ�� ���� �缳���Ǿ���."));
		m_mapDebugErrorDescription.insert(std::make_pair(10055, "�����ִ� ���۰����� ��� ������ ����� �� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10056, "�̹� ������ �Ϸ�� ������. connect�� ������ �Ϸ�� ���Ͽ� �ٽ� connect�� �õ��� ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10057, "������� ���� ������. ������� ���� ���Ͽ� �а� ���� ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10058, "������ ����Ǿ���. ����� ���Ͽ� �����͸� �а� ���� ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10059, "Too many references"));
		m_mapDebugErrorDescription.insert(std::make_pair(10060, "���ӽõ� ���� �ð� �ʰ�"));
		m_mapDebugErrorDescription.insert(std::make_pair(10061, "������ ����õ��� ������. ���� ��⿭�� ������ ���¿��� Ŭ���̾�Ʈ�� connect ȣ���� ���� ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10062, "�ʹ� ���� �ɺ�����ũ�� ���Ǿ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10063, "�����̸��� �ʹ� ��"));
		m_mapDebugErrorDescription.insert(std::make_pair(10064, "ȣ��Ʈ �ٿ�"));
		m_mapDebugErrorDescription.insert(std::make_pair(10065, "ȣ��Ʈ���� ��θ� ������ �� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10066, "��Ʈ��ũ ����ý����� ���� ����� �غ� �Ǿ� ���� ����. WSAStartup�� ���� ��ȯ"));
		m_mapDebugErrorDescription.insert(std::make_pair(10067, "�ʹ� ���� ���μ����� �����Ǿ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10068, "����ڰ� �ʹ� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(10069, "��ũ ��� �Ҵ緮 �ʰ�"));
		m_mapDebugErrorDescription.insert(std::make_pair(10070, "Stale NFS file handle"));
		m_mapDebugErrorDescription.insert(std::make_pair(10071, "Too many levels of remote in path"));
		m_mapDebugErrorDescription.insert(std::make_pair(10101, "���ᰡ ��������"));
		m_mapDebugErrorDescription.insert(std::make_pair(10091, "��Ʈ��ũ ����ý����� ���� ����� �غ� �Ǿ� ���� �ʾ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(10092, "������ �������� �ʴ� �������� ������ �������. WSAStartUp()���� ��ȯ"));
		m_mapDebugErrorDescription.insert(std::make_pair(10093, "WSAStartup() �Լ��� ���������� ������� ���� ��Ȳ���� ���� �Լ��� ȣ������ ��"));
		m_mapDebugErrorDescription.insert(std::make_pair(11001, "��û�� ȣ��Ʈ�� ã�� �� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(11002, "��û�� ȣ��Ʈ�� ã�� �� ����"));
		m_mapDebugErrorDescription.insert(std::make_pair(11003, "������ �� ���� ������ �߻��Ͽ���"));
		m_mapDebugErrorDescription.insert(std::make_pair(11004, "��û�� �̸��� ��ȿ������ ������ ã�� ������. gethostbyaddr()��� �߻�"));
	}
#pragma endregion
	//CLient Interface
	m_hThread = NULL;
	m_hSocket = NULL;

	//Gerneral
	m_hCompletionPort = NULL;
	m_bIsServer = true;
	m_iPort = PORT;
	m_hCompletionPort = NULL;
	m_pListen = null;

	WSADATA tData = {};
	WSAStartup(MAKEWORD(2, 2), &tData);
	
	m_pCommandThread = nullptr;
}


CNetworkManager::~CNetworkManager()
{
	if (m_bIsServer)
	{
		size_t iThreadCount = m_vecThread.size();
		for (size_t i = 0; i < iThreadCount; ++i)
		{
			if (nullptr == m_vecThread[i])
				delete m_vecThread[i];

			m_vecThread[i] = nullptr;
		}

		m_vecThread.clear();

		if (nullptr == m_pListen)
		{
			delete m_pListen;
			m_pListen = nullptr;
		}
	}
	else
	{
		if (m_hThread)
		{
			m_bLoop = false;
			WaitForSingleObject(m_hThread, INFINITE);
			CloseHandle(m_hThread);
		}

		if (m_hSocket)
		{
			shutdown(m_hSocket, SD_BOTH);	
		}

		if (m_pReadPacket)
		{
			delete m_pReadPacket;
		}
		if (m_pWritePacket)
		{
			delete m_pWritePacket;
		}
	}

	delete m_pPacketHub;
	WSACleanup();
}


void CNetworkManager::PrintErrorDescription()
{
	int iErrorCode = WSAGetLastError();

	std::unordered_map<int, std::string>::iterator iter = m_mapDebugErrorDescription.find(iErrorCode);
	if (iter != m_mapDebugErrorDescription.end())
	{
		std::cout << iter->second << std::endl;
	}
}


unsigned int CNetworkManager::ClientLoopProcedure(void * pThis)
{
	while (m_bLoop)
	{

		((CNetworkManager*)pThis)->m_pReadPacket->ClearPacketData();
		recv(((CNetworkManager*)pThis)->GetSocket(), (char*)((CNetworkManager*)pThis)->m_pReadPacket, PACKET_SIZE + 8, 0);
		
		char* pCurrent = (char*)((CNetworkManager*)pThis)->m_pReadPacket;
		
		uint32_t iHeader = 0;
		uint32_t iLength = 0;

		memcpy(&iHeader, pCurrent , sizeof(uint32_t));
		memcpy(&iLength, pCurrent + 4, sizeof(uint32_t));

		CPacket* pNext; 
		CPacket tCurrent;		
		//��Ŷ�� ũ�Ⱑ ���� ���� ������ �߶󳽴�. ��ä ó���ؾ� �ϴ� ��Ŷ�� ������ ó���� ��Ŷ���� �������� �Ѵ�.
		memcpy(&tCurrent, pCurrent, iLength + 8);
		pNext = (CPacket*)(pCurrent + iLength + 8);// �������� ������ ������ �����ϰ� �ݺ��Ǳ� ������, �ּҸ� �ɾ��ֵ��� �Ѵ�.	
		((CNetworkManager*)pThis)->m_pPacketHub->Read(null, &tCurrent);
		

		while (true)
		{			
			pCurrent = (char*)pNext;
			
			iHeader = 0;
			memcpy(&iHeader, pCurrent, sizeof(uint32_t));

			if (iHeader == PACKET_INVALID_HEADER)
				break;

			iLength = 0;
			memcpy(&iLength, pCurrent + 4, sizeof(uint32_t));
			
			tCurrent.ClearPacketData();
			memcpy(&tCurrent, pCurrent, iLength + 8);
			pNext = (CPacket*)(pCurrent + iLength + 8); 		
			((CNetworkManager*)pThis)->m_pPacketHub->Read(null, &tCurrent);

			//���� �߰ߵǴ� ���� 4����Ʈ�� 0�� ���, �� ����� ��ȿ���� ���� ��� �����ϵ��� �Ѵ�.
		}
	}

	return 0;
}

SOCKET CNetworkManager::GetSocket() const
{
	return m_hSocket;
}

CPacket * CNetworkManager::GetReadPacket()
{
	return m_pReadPacket;
}
CPacket * CNetworkManager::GetWritePacket()
{
	return m_pWritePacket;
}

void CNetworkManager::SendToServer()
{
	if (m_pWritePacket)
	{
		send(m_hSocket, (char*)m_pWritePacket, m_pWritePacket->GetSize() + 8, 0);
		m_pWritePacket->ClearPacketData();
	}
}


bool CNetworkManager::Initialize(bool isServer, std::string targetServerAddress)
{
	m_bIsServer = isServer;
	if (isServer)
	{

		m_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
		
		//Listen Session ����
		//���� ��û�� �޾��ִ� ������ �Ѵ�.
		m_pListen = new CNetworkSession;
		m_pListen->CreateListen(PORT);
		

		SYSTEM_INFO tInfo;
		GetSystemInfo(&tInfo);
		
		for (size_t i = 0; i < tInfo.dwNumberOfProcessors - 1; ++i)
		{
			CNetworkThread* pThread = new CNetworkThread;
			pThread->SetCompletionPort(m_hCompletionPort);
			pThread->CreateThread();
			m_vecThread.push_back(pThread);
		}
	}
	else
	{
		if (!targetServerAddress.empty())
		{
			m_pReadPacket = new CPacket;
			m_pWritePacket = new CPacket;
			m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			SOCKADDR_IN tAddress = {};
			tAddress.sin_family = AF_INET;
			tAddress.sin_port = htons(PORT);
			tAddress.sin_addr.s_addr = inet_addr(targetServerAddress.c_str());

			connect(m_hSocket, (SOCKADDR*)&tAddress, sizeof(tAddress));

			m_hThread = (HANDLE)_beginthreadex(0, 0, CNetworkManager::ClientLoopProcedure, this, 0, 0);
		}
		else
			return false;
	}
	if (WSAGetLastError() != 0)
	{
		PrintErrorDescription();
		return false;
	}
	return true;
}

void CNetworkManager::Update()
{
	if (m_bIsServer)
	{
		while (true)
		{
			m_pListen->Accept();
		}
	}
}

void CNetworkManager::AddClient(SOCKET hSocket, CNetworkSession * pSession)
{
	std::unordered_map<SOCKET, CNetworkSession*>::iterator iter = m_mapClientSession.find(hSocket);

	if (iter == m_mapClientSession.end())
	{
		m_mapClientSession.insert(std::make_pair(hSocket, pSession));
	}
	else
	{
		OutputDebugString(TEXT("�̹� ����Ǿ� �ִ� Ŭ���̾�Ʈ�� �ٽ� ������ �õ��մϴ�."));
	}
}

void CNetworkManager::RemoveClient(SOCKET hSocket)
{
	std::unordered_map<SOCKET, CNetworkSession*>::iterator iter = m_mapClientSession.find(hSocket);

	if (iter != m_mapClientSession.end())
	{
		m_mapClientSession.erase(iter);
	}
	else
	{
		OutputDebugString(TEXT("������ �̹� ������ Ŭ���̾�Ʈ�̰ų�, �������� �ʴ� Ŭ���̾�Ʈ�Դϴ�."));
	}
}

const CNetworkSession * CNetworkManager::GetClientSession(SOCKET hSocket) const
{
	std::unordered_map<SOCKET, CNetworkSession*>::const_iterator iter = m_mapClientSession.find(hSocket);
	if (iter != m_mapClientSession.end())
	{
		return iter->second;
	}
	return nullptr;
}


SHORT CNetworkManager::GetPort() const
{
	return m_iPort;
}

int32_t CNetworkManager::GetPingTime(CNetworkSession * user) const
{
	return -1;
}

int32_t CNetworkManager::GetReturnTripTime(CNetworkSession * user) const
{
	return -1;
}

int32_t CNetworkManager::GetPingTime(SOCKET user) const
{
	std::unordered_map<SOCKET, CNetworkSession* >::const_iterator iter = m_mapClientSession.find(user);
	if (iter != m_mapClientSession.end())
	{
		//��� �۾� ����
		iter->first;
	}

	return -1;
}

int32_t CNetworkManager::GetReturnTripTime(SOCKET user) const
{
	return -1;
}

void CNetworkManager::ConnectIOCP(CNetworkSession * pSession) const
{
	CreateIoCompletionPort((HANDLE)pSession->GetSocket(), m_hCompletionPort, (ULONG_PTR)pSession, 0);
}
