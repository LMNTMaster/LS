#include "NetworkThread.h"
#include "NetworkSession.h"
#include "NetworkManager.h"
#include "Packet.h"



CNetworkThread::CNetworkThread() :
	m_hCompletionPort(NULL)
{
}


CNetworkThread::~CNetworkThread()
{
}

void CNetworkThread::SetCompletionPort(HANDLE hCP)
{
	m_hCompletionPort = hCP;
}

void CNetworkThread::RunThread()
{
	DWORD dwTransfer = 0;
	CNetworkSession* pSession = NULL;
	CNetworkBuffer* pData;

	while (true)
	{
		/*
		--인자들--
		CompletionPort						:	완료된 IO 정보가 등록되어 있는 CP오브젝트의 핸들
		lpNumberOfBytesTransferred	:	입출력 과정에서 송수신 된 데이터의 크기 정보를 저장할 변수의 주소
		lpCompletionKey						:	전달 된 값의 저장을 위한 변수의 주소
		lpOverlapped							:	WSASend, WSARecv 함수 호출 시 전달하는 OVERLAPPED 구조체 변수의 주소
		dwMilliseconds						:	타임아웃 정보 전달. 여기서 지정한 시간이 만료되면, FALSE를 반환하면서, 함수를 빠져나간다.
		INFINITE를 전달하면 완료된 IO가 CP오브젝트에 등록될 떄까지 블로킹 상태에 있게 된다.
		*/
		GetQueuedCompletionStatus(m_hCompletionPort, &dwTransfer, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&pData, INFINITE);

		SOCKET hSocket = pSession->GetSocket();

		//Read일 경우

		if (pData->m_bRead)
		{
			if (dwTransfer == 0)
			{
				delete pSession;
				continue;
			}
			else
			{
				//읽고
				CPacket* pPacket = pSession->GetReadPacket();
				CNetworkManager::GetInstance()->m_pPacketHub->Read(pSession, pPacket);
				
				//Echo
				//unordered_map<SOCKET, CNetworkSession*>* setSession = CNetworkManager::GetInstance()->GetClientSession();
				//unordered_map<SOCKET, CNetworkSession*>::iterator iter;
				//unordered_map<SOCKET, CNetworkSession*>::iterator iterEnd = setSession->end();
				//for (iter = setSession->begin(); iter != iterEnd; ++iter)
				//{
				//	(*iter).second->Write(tPacket.iHeader, tPacket.iSize, tPacket.pData);
				//}
			}
			//읽도록 대기시킴
			pSession->Read();
		}
		//Send일 경우
		else
		{
		}
	}
}
