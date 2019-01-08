#include "Packet.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include <cstring>
#include <cwchar>
#include "Socket.hpp"

namespace acid
{
	Packet::Packet()
	{
	}

	std::pair<const void *, std::size_t> Packet::OnSend()
	{
		return std::make_pair(GetData(), GetDataSize());
	}

	void Packet::OnReceive(const void *data, const std::size_t &size)
	{
		Append(data, size);
	}
}
