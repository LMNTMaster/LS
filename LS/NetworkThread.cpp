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
		GetQueuedCompletionStatus(m_hCompletionPort, &dwTransfer, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&pData, INFINITE);
		SOCKET hSocket = pSession->GetSocket();

		if (pData->m_bRead)
		{
			if (dwTransfer == 0)
			{
				delete pSession;
				continue;
			}
			else
			{
				CPacket* pPacket = pSession->GetReadPacket();
				CNetworkManager::GetInstance()->m_pPacketHub->Read(pSession, pPacket);				
			}			
			pSession->Read();//읽도록 대기시킴
		}
		else
		{
			/*Send*/
		}
	}
}
