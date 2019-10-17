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
	std::cout << "게임중.." << std::endl;
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
		std::cout << "얘 함수 호출한대는데?" << std::endl;
		pSession->Write("안된다고 해도 할거자나 그니까 돼.");
		pSession->Write(pPacket->GetPacketHeader(), pPacket->GetSize(), pPacket->GetBuffer());
		break;

	case PACKET_STRING_MESSAGE:
		pSession->Write("돼.");
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