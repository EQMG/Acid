#include "TcpListener.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif
#include "Engine/Log.hpp"
#include "TcpSocket.hpp"

namespace acid
{
	TcpListener::TcpListener() :
		Socket(Type::Tcp)
	{
	}

	uint16_t TcpListener::GetLocalPort() const
	{
		if (GetHandle() != Socket::InvalidSocketHandle())
		{
			// Retrieve informations about the local end of the socket.
			sockaddr_in address;
			SocketAddrLength size = sizeof(address);

			if (getsockname(GetHandle(), reinterpret_cast<sockaddr *>(&address), &size) != -1)
			{
				return ntohs(address.sin_port);
			}
		}

		// We failed to retrieve the port.
		return 0;
	}

	Socket::Status TcpListener::Listen(const uint16_t &port, const IpAddress &address)
	{
		// Close the socket if it is already bound.
		Close();

		// Create the internal socket if it doesn't exist.
		Create();

		// Check if the address is valid.
		if ((address == IpAddress::None) || (address == IpAddress::Broadcast))
		{
			return Socket::Status::Error;
		}

		// Bind the socket to the specified port.
		sockaddr_in addr = Socket::CreateAddress(address.ToInteger(), port);

		if (bind(GetHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
		{
			// Not likely to happen, but...
			Log::Error("Failed to bind listener socket to port %i\n", port);
			return Socket::Status::Error;
		}

		// Listen to the bound port.
		if (listen(GetHandle(), SOMAXCONN) == -1)
		{
			// Oops, socket is deaf.
			Log::Error("Failed to listen to port %i\n", port);
			return Socket::Status::Error;
		}

		return Socket::Status::Done;
	}

	void TcpListener::Close()
	{
		// Simply close the socket.
		Socket::Close();
	}

	Socket::Status TcpListener::Accept(TcpSocket &socket)
	{
		// Make sure that we're listening.
		if (GetHandle() == Socket::InvalidSocketHandle())
		{
			Log::Error("Failed to accept a new connection, the socket is not listening\n");
			return Socket::Status::Error;
		}

		// Accept a new connection.
		sockaddr_in address;
		SocketAddrLength length = sizeof(address);
		SocketHandle remote = accept(GetHandle(), reinterpret_cast<sockaddr *>(&address), &length);

		// Check for errors.
		if (remote == Socket::InvalidSocketHandle())
		{
			return Socket::GetErrorStatus();
		}

		// Initialize the new connected socket.
		socket.Close();
		socket.Create(remote);

		return Socket::Status::Done;
	}
}
