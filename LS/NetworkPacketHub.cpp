#include "NetworkPacketHub.h"
#include "Packet.h"
#include "NetworkSession.h"

CNetworkPacketHub::CNetworkPacketHub()
{
}

CNetworkPacketHub::~CNetworkPacketHub()
{
}

void CNetworkPacketHub::Read(CNetworkSession* pSession, CPacket* pPacket)
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
