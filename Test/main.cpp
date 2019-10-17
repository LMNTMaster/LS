#include <NetworkManager.h>
#include <NetworkPacketHub.h>
#include <NetworkSession.h>
#include <Packet.h>
#include <CommandThread.h>
#pragma comment(lib, "LS")

class CGame : public CCommandThread
{
public:
	virtual void RunThread() override;
};

void CGame::RunThread()
{
	CEngine::Play();
}


class CEngine
{
public:
	static void Play();
};

void CEngine::Play()
{
	std::cout << "������.." << std::endl;
}


class CDecoder : public CNetworkPacketHub
{
public:
	virtual void Read(CNetworkSession* pSession, CPacket* pPacket) override;
};

void CDecoder::Read(CNetworkSession* pSession, CPacket* pPacket)
{
	switch (pPacket->GetPacketHeader())
	{
	case PACKET_FUCNTION_CALL:
		std::cout << "�� �Լ� ȣ���Ѵ�µ�?" << std::endl;
		pSession->Write("�ȵȴٰ� �ص� �Ұ��ڳ� �״ϱ� ��.");
		pSession->Write(pPacket->GetPacketHeader(), pPacket->GetSize(), pPacket->GetBuffer());
		break;

	case PACKET_STRING_MESSAGE:
		pSession->Write("��.");
		break;
	}
}

void main()
{
	CNetworkManager* pManager = CNetworkManager::GetInstance();

	if (pManager)
	{
		if (pManager->Initialize(true))
		{
			pManager->SetDecoderClass<CDecoder>();
			pManager->SetCommandFunction<CGame>();
			pManager->Update();
		}
	}
	CNetworkManager::DestroyInstance();
}