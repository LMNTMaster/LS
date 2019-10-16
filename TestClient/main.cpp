#include <NetworkManager.h>
#pragma comment(lib, "LS")
#include <Packet.h>
#include <vector>

class CDecoder : public CNetworkPacketHub
{
public:
	virtual void Read(CNetworkSession* pSession, CPacket* pPacket) override;
};

void CDecoder::Read(CNetworkSession* pSession, CPacket* pPacket)
{
	switch (pPacket->GetPacketHeader())
	{
	case PACKET_FUCNTION_CALL :
	{
		void* fpAddress = nullptr;
		memcpy(&fpAddress, pPacket->GetBuffer(), sizeof(void*));
		void* pThis = nullptr;
		memcpy(&pThis, (char*)(pPacket->GetBuffer()) + sizeof(void*), sizeof(void*));
		((int(*)(void*))fpAddress)(pThis);
	}
		break;
	case PACKET_STRING_MESSAGE :
	{
		std::cout << pPacket->GetBuffer() << std::endl;
	}
		break;

	}
}

int Add(void* pTest)
{
	std::cout << "�����մϴ�!!!" << "\n\n���ߴ�!!!" << std::endl;
	return 0;
}


void main()
{
	int(*fpAdd)(void*) = Add;

	CNetworkManager* pManager = CNetworkManager::GetInstance();

	bool aa = std::is_convertible<CDecoder, CNetworkPacketHub>();
	if (pManager)
	{
		//if (pManager->Initialize(false, "121.168.225.13"))
		if (pManager->Initialize(false, "127.0.0.1"))
		{
			pManager->SetDecoderClass<CDecoder>();
			pManager->Update();
		}
	}
	
	CPacket* pPacket = pManager->GetWritePacket();		
	std::cout << "������ �Ҷ����! ���ϱ� �Լ��� ȣ���ص� �ɱ��!!!!" << std::endl;
	pPacket->SetPacketHeader(PACKET_FUCNTION_CALL);
	pPacket->PushData(fpAdd, 0);
	pManager->SendToServer();


	//PACKET_FUCNTION_CALL�� ���, ������ ���� ���¸� �Ϲ������� ����ϵ��� �ϰ�, �ִ� 32�� ���� �Է� �����ϵ��� �Ѵ�.
	//(4:fpa, 1 : read count, 4 : input mask,  4 : output mask, )
	//pPacket->PushData(fpAdd, 2, sizeof(int), 5243, sizeof(int), 4325);

	int i = 0;
	std::cin >> i;
	CNetworkManager::DestroyInstance();
}