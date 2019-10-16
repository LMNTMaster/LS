#include "Packet.h"



CPacket::CPacket()
{
	ClearPacketData();
}


CPacket::~CPacket()
{
}

void CPacket::ClearPacketData()
{
	memset(&m_pData[0], 0, PACKET_SIZE);
	m_iSize = 0;
	m_iHeader = 0;
}

void CPacket::SetPacketHeader(uint32_t iHeader)
{
	m_iHeader = iHeader;
}

uint32_t CPacket::GetPacketHeader() const
{
	return m_iHeader;
}

uint32_t CPacket::GetSize() const
{
	return m_iSize;
}

char * CPacket::GetBuffer()
{
	return &m_pData[0];
}
