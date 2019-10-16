#pragma once
#include "Core.h"
class LMNT_DLL CPacket
{
public:
	CPacket();
	~CPacket();

private:
	uint32_t	m_iHeader;
	uint32_t	m_iSize;
	char			m_pData[PACKET_SIZE];

public:
	void ClearPacketData();
	void SetPacketHeader(uint32_t iHeader);
	uint32_t GetPacketHeader() const;
	
	uint32_t GetSize() const;
	char* GetBuffer();



	void PushData(const char* data, size_t iSize)
	{
		if(m_iSize + iSize <= PACKET_SIZE)
			memcpy(&m_pData[m_iSize], data, iSize);

		m_iSize += iSize;
	}
	
	
	template<typename T, typename ... Types>
	void PushData(const T& data, const Types& ... others)
	{
		if (m_iSize + sizeof(T) <= PACKET_SIZE)
			memcpy(&m_pData[m_iSize], &data, sizeof(T));

		m_iSize += sizeof(T);
		
		PushData(others...);
	}void PushData() {}

	template<typename ... Types>
	void PushData(const char* data, const Types& ... others)
	{
		uint32_t iLength = static_cast<size_t>(strlen(data));
		if (m_iSize + iLength <= PACKET_SIZE)
			memcpy(&m_pData[m_iSize], data, iLength);

		m_iSize += iLength;

		PushData(others...);
	}


	template<typename ... Types>
	void PushData(const std::string& data, const Types& ... others)
	{
		uint32_t iLength = data.size();
		if (m_iSize + iLength <= PACKET_SIZE)
			memcpy(&m_pData[m_iSize], &data[0], iLength);

		m_iSize += iLength;

		PushData(others...);
	}
};

