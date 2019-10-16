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
		--���ڵ�--
		CompletionPort						:	�Ϸ�� IO ������ ��ϵǾ� �ִ� CP������Ʈ�� �ڵ�
		lpNumberOfBytesTransferred	:	����� �������� �ۼ��� �� �������� ũ�� ������ ������ ������ �ּ�
		lpCompletionKey						:	���� �� ���� ������ ���� ������ �ּ�
		lpOverlapped							:	WSASend, WSARecv �Լ� ȣ�� �� �����ϴ� OVERLAPPED ����ü ������ �ּ�
		dwMilliseconds						:	Ÿ�Ӿƿ� ���� ����. ���⼭ ������ �ð��� ����Ǹ�, FALSE�� ��ȯ�ϸ鼭, �Լ��� ����������.
		INFINITE�� �����ϸ� �Ϸ�� IO�� CP������Ʈ�� ��ϵ� ������ ���ŷ ���¿� �ְ� �ȴ�.
		*/
		GetQueuedCompletionStatus(m_hCompletionPort, &dwTransfer, (PULONG_PTR)&pSession, (LPOVERLAPPED*)&pData, INFINITE);

		SOCKET hSocket = pSession->GetSocket();

		//Read�� ���

		if (pData->m_bRead)
		{
			if (dwTransfer == 0)
			{
				delete pSession;
				continue;
			}
			else
			{
				//�а�
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
			//�е��� ����Ŵ
			pSession->Read();
		}
		//Send�� ���
		else
		{
		}
	}
}
