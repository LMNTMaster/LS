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
		m_mapDebugErrorDescription.insert(std::make_pair(6, "지정된 이벤트 객체 핸들이 잘못 되었다."));
		m_mapDebugErrorDescription.insert(std::make_pair(8, "메모리가 충분하지 않다."));
		m_mapDebugErrorDescription.insert(std::make_pair(87, "하나 이상의 잘못된 인자가 사용됐다."));
		m_mapDebugErrorDescription.insert(std::make_pair(995, "overlapped(:12) 연산이 중단 되었다."));
		m_mapDebugErrorDescription.insert(std::make_pair(996, "Overlapped 입출력 이벤트가 신호 상태가 아니다."));
		m_mapDebugErrorDescription.insert(std::make_pair(997, "Overlapped 연산은 나중에 완료될 것이다. 중첩 연산을 위한 준비가 되었으나, 즉시 완료되지 않았을 경우발생"));
		m_mapDebugErrorDescription.insert(std::make_pair(10004, "WSACancelBlockingCall()에 의해 블록화 호출이 취소: Interrupted system call"));
		m_mapDebugErrorDescription.insert(std::make_pair(10009, "잘못된 파일 기술자가 사용되었음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10013, "요청한 주소가 브로드캐스트 주소인데 setsockopt()로 SO_BROADCAST 가 설정되지 않았다."));
		m_mapDebugErrorDescription.insert(std::make_pair(10014, "잘못된 주소를 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10022, "바인딩 실패. 이미 bind된 소켓에 바인드하거나 주소체계가 일관적이지 않았다.\n 또는 listen() 호출 전에 accept()가 호출되었거나, 초기화기 등록된 Completion Routine이 NULL이 아니다."));
		m_mapDebugErrorDescription.insert(std::make_pair(10024, "너무 많은 파일이 열려있음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10035, "non overlapped 소켓 : 비 봉쇄 소켓에 아직 읽을 데이터가 없음, overlapped(:12) 소켓 : 너무 많은 중첩 입출력 요구가 있음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10036, "블록화 함수가 진행되는 동안 부적절한 윈속 API함수가 호출."));
		m_mapDebugErrorDescription.insert(std::make_pair(10037, "비봉쇄모드 소켓에서 아직 진행중인 작업임. (connect가 완료되지 않은 상태에서 connect 함수의 호출등..)"));
		m_mapDebugErrorDescription.insert(std::make_pair(10038, "잘못된 소켓기술자를 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10039, "목적지 주소를 명시하지 않았음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10040, "송수신에 사용된 데이터가 버퍼의 크기를 초과해서 크기에 맞게 잘렸음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10041, "소켓에 맞지 않는 프로토콜을 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10042, "잘못된 옵션을 사용했음. 지원되지 않는 옵션으로 getsockopt함수를 호출하는 등."));
		m_mapDebugErrorDescription.insert(std::make_pair(10043, "윈속에서 지원하지 않는 주소체계를 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10044, "소켓타입이 지원하지 않는 주소체계를 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10045, "소켓이 지원하지 않는 명령을 사용했음. listen()함수를 데이터그램 통신 (SO_DGRAM)에서 호출"));
		m_mapDebugErrorDescription.insert(std::make_pair(10046, "지원하지 않는 프로토콜을 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10047, "윈속에서 지원하지 않는 주소체계를 사용했음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10048, "지정된 주소가 이미 사용 중임"));
		m_mapDebugErrorDescription.insert(std::make_pair(10049, "사용할 수 없는 주소임"));
		m_mapDebugErrorDescription.insert(std::make_pair(10050, "네트워크 서브 시스템에 문제가 있음. 네트워크 접속 끊김등."));
		m_mapDebugErrorDescription.insert(std::make_pair(10051, "네크워크에 접근할 수 없음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10052, "네트워크 재설정으로 연결이 끊어졌음."));
		m_mapDebugErrorDescription.insert(std::make_pair(10053, "타임아웃 혹은 상대방의 접속종료들과 같은 소프트웨어적인 문제로 연결이 끊겼음."));
		m_mapDebugErrorDescription.insert(std::make_pair(10054, "연결이 원격 호스트에 의해 재설정되었음."));
		m_mapDebugErrorDescription.insert(std::make_pair(10055, "남아있는 버퍼공간이 없어서 소켓을 사용할 수 없음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10056, "이미 연결이 완료된 소켓임. connect로 연결이 완료된 소켓에 다시 connect를 시도할 경우"));
		m_mapDebugErrorDescription.insert(std::make_pair(10057, "연결되지 않은 소켓임. 연결되지 않은 소켓에 읽고 쓰는 경우"));
		m_mapDebugErrorDescription.insert(std::make_pair(10058, "소켓이 종료되었음. 종료된 소켓에 데이터를 읽고 쓰려 경우"));
		m_mapDebugErrorDescription.insert(std::make_pair(10059, "Too many references"));
		m_mapDebugErrorDescription.insert(std::make_pair(10060, "접속시도 제한 시간 초과"));
		m_mapDebugErrorDescription.insert(std::make_pair(10061, "서버가 연결시도를 거절함. 수신 대기열이 가득찬 상태에서 클라이언트의 connect 호출이 있을 경우"));
		m_mapDebugErrorDescription.insert(std::make_pair(10062, "너무 많은 심볼릭링크가 사용되었음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10063, "파일이름이 너무 김"));
		m_mapDebugErrorDescription.insert(std::make_pair(10064, "호스트 다운"));
		m_mapDebugErrorDescription.insert(std::make_pair(10065, "호스트로의 경로를 설정할 수 없음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10066, "네트워크 서브시스템이 아직 통신할 준비가 되어 있지 않음. WSAStartup에 의해 반환"));
		m_mapDebugErrorDescription.insert(std::make_pair(10067, "너무 많은 프로세스가 생성되었음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10068, "사용자가 너무 많음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10069, "디스크 허용 할당량 초과"));
		m_mapDebugErrorDescription.insert(std::make_pair(10070, "Stale NFS file handle"));
		m_mapDebugErrorDescription.insert(std::make_pair(10071, "Too many levels of remote in path"));
		m_mapDebugErrorDescription.insert(std::make_pair(10101, "종료가 진행중임"));
		m_mapDebugErrorDescription.insert(std::make_pair(10091, "네트워크 서브시스템이 아직 통신할 준비가 되어 있지 않았음"));
		m_mapDebugErrorDescription.insert(std::make_pair(10092, "윈속이 지원하지 않는 프로토콜 버전을 사용했음. WSAStartUp()에서 반환"));
		m_mapDebugErrorDescription.insert(std::make_pair(10093, "WSAStartup() 함수가 성공적으로 실행되지 않은 상황에서 윈속 함수를 호출했을 때"));
		m_mapDebugErrorDescription.insert(std::make_pair(11001, "요청된 호스트를 찾을 수 없음"));
		m_mapDebugErrorDescription.insert(std::make_pair(11002, "요청된 호스트를 찾을 수 없음"));
		m_mapDebugErrorDescription.insert(std::make_pair(11003, "복구할 수 없는 에러가 발생하였음"));
		m_mapDebugErrorDescription.insert(std::make_pair(11004, "요청한 이름이 유효하지만 정보를 찾지 못했음. gethostbyaddr()등에서 발생"));
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
		//패킷이 크기가 남지 않을 떄까지 잘라낸다. 현채 처리해야 하는 패킷과 다음에 처리할 패킷으로 나누도록 한다.
		memcpy(&tCurrent, pCurrent, iLength + 8);
		pNext = (CPacket*)(pCurrent + iLength + 8);// 나머지는 구조가 완전히 동일하게 반복되기 때문에, 주소만 걸어주도록 한다.	
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

			//이후 발견되는 최초 4바이트가 0일 경우, 즉 헤더가 유효하지 않은 경우 종료하도록 한다.
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
		
		//Listen Session 생성
		//연결 요청을 받아주는 역할을 한다.
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
		OutputDebugString(TEXT("이미 연결되어 있는 클라이언트가 다시 연결을 시도합니다."));
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
		OutputDebugString(TEXT("접속을 이미 종료한 클라이언트이거나, 존재하지 않는 클라이언트입니다."));
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
		//얘로 작업 시작
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
