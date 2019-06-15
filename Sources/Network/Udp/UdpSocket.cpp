#include "UdpSocket.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif
#include "Engine/Log.hpp"
#include "Network/IpAddress.hpp"
#include "Network/Packet.hpp"

namespace acid
{
static const uint32_t MAX_DATAGRAM_SIZE{65507};

UdpSocket::UdpSocket() :
	Socket{Type::Udp},
	m_buffer(MAX_DATAGRAM_SIZE)
{
}

uint16_t UdpSocket::GetLocalPort() const
{
	if (GetHandle() != InvalidSocketHandle())
	{
		// Retrieve informations about the local end of the socket.
		sockaddr_in address;
		SocketAddrLength size{sizeof(address)};

		if (getsockname(GetHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
		{
			return ntohs(address.sin_port);
		}
	}

	// We failed to retrieve the port.
	return 0;
}

Socket::Status UdpSocket::Bind(const uint16_t &port, const IpAddress &address)
{
	// Close the socket if it is already bound.
	Close();

	// Create the internal socket if it doesn't exist.
	Create();

	// Check if the address is valid/
	if ((address == IpAddress::None) || (address == IpAddress::Broadcast))
	{
		return Status::Error;
	}

	// Bind the socket/
	auto addr{CreateAddress(address.ToInteger(), port)};

	if (bind(GetHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
	{
		Log::Error("Failed to bind socket to port %i\n", port);
		return Status::Error;
	}

	return Status::Done;
}

void UdpSocket::Unbind()
{
	// Simply close the socket.
	Close();
}

Socket::Status UdpSocket::Send(const void *data, const std::size_t &size, const IpAddress &remoteAddress, const uint16_t &remotePort)
{
	// Create the internal socket if it doesn't exist.
	Create();

	// Make sure that all the data will fit in one datagram.
	if (size > MAX_DATAGRAM_SIZE)
	{
		Log::Error("Cannot send data over the network (the number of bytes to send is greater than UdpSocket::MAX_DATAGRAM_SIZE)\n");
		return Status::Error;
	}

	// Build the target address.
	auto address{CreateAddress(remoteAddress.ToInteger(), remotePort)};

	// Send the data (unlike TCP, all the data is always sent in one call).
	auto sent{sendto(GetHandle(), static_cast<const char *>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr *>(&address), sizeof(address))};

	// Check for errors.
	if (sent < 0)
	{
		return GetErrorStatus();
	}

	return Status::Done;
}

Socket::Status UdpSocket::Receive(void *data, const std::size_t &size, std::size_t &received, IpAddress &remoteAddress, uint16_t &remotePort)
{
	// First clear the variables to fill.
	received = 0;
	remoteAddress = IpAddress();
	remotePort = 0;

	// Check the destination buffer.
	if (!data)
	{
		Log::Error("Cannot receive data from the network (the destination buffer is invalid)\n");
		return Status::Error;
	}

	// Data that will be filled with the other computer's address.
	auto address{CreateAddress(INADDR_ANY, 0)};

	// Receive a chunk of bytes.
	SocketAddrLength addressSize{sizeof(address)};
	auto sizeReceived{recvfrom(GetHandle(), static_cast<char *>(data), static_cast<int>(size), 0, reinterpret_cast<sockaddr *>(&address), &addressSize)};

	// Check for errors.
	if (sizeReceived < 0)
	{
		return GetErrorStatus();
	}

	// Fill the sender informations.
	received = static_cast<std::size_t>(sizeReceived);
	remoteAddress = IpAddress(ntohl(address.sin_addr.s_addr));
	remotePort = ntohs(address.sin_port);

	return Status::Done;
}

Socket::Status UdpSocket::Send(Packet &packet, const IpAddress &remoteAddress, const uint16_t &remotePort)
{
	// UDP is a datagram-oriented protocol (as opposed to TCP which is a stream protocol).
	// Sending one datagram is almost safe: it may be lost but if it's received, then its data
	// is guaranteed to be ok. However, splitting a packet into multiple datagrams would be highly
	// unreliable, since datagrams may be reordered, dropped or mixed between different sources.
	// That's why a limit on packet size so that they can be sent in a single datagram.
	// This also removes the overhead associated to packets -- there's no size to send in addition
	// to the packet's data.

	// Get the data to send from the packet.
	auto dataSize{packet.OnSend()};

	// Send it.
	return Send(dataSize.first, dataSize.second, remoteAddress, remotePort);
}

Socket::Status UdpSocket::Receive(Packet &packet, IpAddress &remoteAddress, uint16_t &remotePort)
{
	// See the detailed comment in send(Packet) above.

	// Receive the datagram.
	std::size_t received{};
	auto status{Receive(&m_buffer[0], m_buffer.size(), received, remoteAddress, remotePort)};

	// If we received valid data, we can copy it to the user packet.
	packet.Clear();

	if ((status == Status::Done) && (received > 0))
	{
		packet.OnReceive(&m_buffer[0], received);
	}

	return status;
}
}
