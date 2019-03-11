#include "IpAddress.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include "Http/Http.hpp"
#include "Socket.hpp"

namespace acid
{
	const IpAddress IpAddress::None = IpAddress();
	const IpAddress IpAddress::Any = IpAddress(0, 0, 0, 0);
	const IpAddress IpAddress::LocalHost = IpAddress(127, 0, 0, 1);
	const IpAddress IpAddress::Broadcast = IpAddress(255, 255, 255, 255);

	IpAddress::IpAddress() :
		m_address(0),
		m_valid(false)
	{
	}

	IpAddress::IpAddress(const std::string &address) :
		m_address(0),
		m_valid(false)
	{
		Resolve(address);
	}

	IpAddress::IpAddress(const uint8_t &byte0, const uint8_t &byte1, const uint8_t &byte2, const uint8_t &byte3) :
		m_address(htonl((byte0 << 24) | (byte1 << 16) | (byte2 << 8) | byte3)),
		m_valid(true)
	{
	}

	IpAddress::IpAddress(const uint32_t &address) :
		m_address(htonl(address)),
		m_valid(true)
	{
	}

	std::string IpAddress::ToString() const
	{
		in_addr address = {};
		address.s_addr = m_address;
		return inet_ntoa(address);
	}

	uint32_t IpAddress::ToInteger() const
	{
		return ntohl(m_address);
	}

	IpAddress IpAddress::GetLocalAddress()
	{
		// The method here is to connect a UDP socket to anyone (here to localhost),
		// and get the local socket address with the getsockname function.
		// UDP connection will not send anything to the network, so this function won't cause any overhead.

		IpAddress localAddress;

		// Create the socket.
		SocketHandle sock = socket(PF_INET, SOCK_DGRAM, 0);

		if (sock == Socket::InvalidSocketHandle())
		{
			return localAddress;
		}

		// Connect the socket to localhost on any port.
		sockaddr_in address = Socket::CreateAddress(ntohl(INADDR_LOOPBACK), 9);

		if (connect(sock, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == -1)
		{
			Socket::CloseSocketHandle(sock);
			return localAddress;
		}

		// Get the local address of the socket connection.
		SocketAddrLength size = sizeof(address);

		if (getsockname(sock, reinterpret_cast<sockaddr *>(&address), &size) == -1)
		{
			Socket::CloseSocketHandle(sock);
			return localAddress;
		}

		// Close the socket.
		Socket::CloseSocketHandle(sock);

		// Finally build the IP address.
		localAddress = IpAddress(ntohl(address.sin_addr.s_addr));

		return localAddress;
	}

	IpAddress IpAddress::GetPublicAddress(const Time &timeout)
	{
		// The trick here is more complicated, because the only way
		// to get our public IP address is to get it from a distant computer.
		// Here we get the web page from http://www.sfml-dev.org/ip-provider.php
		// and parse the result to extract our IP address
		// (not very hard: the web page contains only our IP address).

		Http server = Http("www.sfml-dev.org");
		HttpRequest request("/ip-provider.php", HttpRequest::Method::Get);
		HttpResponse page = server.SendRequest(request, timeout);

		if (page.GetStatus() == HttpResponse::Status::Ok)
		{
			return IpAddress(page.GetBody());
		}

		// Something failed: return an invalid address.
		return IpAddress();
	}

	bool IpAddress::operator==(const IpAddress &other) const
	{
		return !(*this < other) && !(other < *this);
	}

	bool IpAddress::operator!=(const IpAddress &other) const
	{
		return !(*this == other);
	}

	bool IpAddress::operator<(const IpAddress &other) const
	{
		return std::make_pair(m_valid, m_address) < std::make_pair(other.m_valid, other.m_address);
	}

	bool IpAddress::operator<=(const IpAddress &other) const
	{
		return !(other < *this);
	}

	bool IpAddress::operator>(const IpAddress &other) const
	{
		return other < *this;
	}

	bool IpAddress::operator>=(const IpAddress &other) const
	{
		return !(*this < other);
	}

	std::ostream &operator<<(std::ostream &stream, const IpAddress &address)
	{
		return stream << address.ToString();
	}

	std::istream &operator>>(std::istream &stream, IpAddress &address)
	{
		std::string str;
		stream >> str;
		address = IpAddress(str);

		return stream;
	}

	void IpAddress::Resolve(const std::string &address)
	{
		m_address = 0;
		m_valid = false;

		if (address == "255.255.255.255")
		{
			// The broadcast address needs to be handled explicitly, because it is also the value returned by inet_addr on error.
			m_address = INADDR_BROADCAST;
			m_valid = true;
		}
		else if (address == "0.0.0.0")
		{
			m_address = INADDR_ANY;
			m_valid = true;
		}
		else
		{
			// Try to convert the address as a byte representation ("xxx.xxx.xxx.xxx").
			uint32_t ip = inet_addr(address.c_str());

			if (ip != INADDR_NONE)
			{
				m_address = ip;
				m_valid = true;
			}
			else
			{
				// Not a valid address, try to convert it as a host name.
				addrinfo hints = {};
				hints.ai_family = AF_INET;
				addrinfo *result = nullptr;

				if (getaddrinfo(address.c_str(), nullptr, &hints, &result) == 0)
				{
					if (result)
					{
						ip = reinterpret_cast<sockaddr_in *>(result->ai_addr)->sin_addr.s_addr;
						freeaddrinfo(result);
						m_address = ip;
						m_valid = true;
					}
				}
			}
		}
	}
}
