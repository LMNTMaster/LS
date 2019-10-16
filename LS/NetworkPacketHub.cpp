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
		std::cout << "얘 함수 호출한대는데?" << std::endl;
		pSession->Write("안된다고 해도 할거자나 그니까 돼.");
		pSession->Write(pPacket->GetPacketHeader(), pPacket->GetSize(), pPacket->GetBuffer());
		break;

	case PACKET_STRING_MESSAGE:
		pSession->Write("돼.");
		break;
	}
}
