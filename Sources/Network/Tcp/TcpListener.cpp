#include "TcpListener.hpp"

#include "Engine/Log.hpp"
#include "TcpSocket.hpp"

namespace acid
{
	TcpListener::TcpListener() :
		Socket(SOCKET_TYPE_TCP)
	{
	}

	unsigned short TcpListener::GetLocalPort() const
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

	SocketStatus TcpListener::Listen(unsigned short port, const IpAddress &address)
	{
		// Close the socket if it is already bound.
		Close();

		// Create the internal socket if it doesn't exist.
		Create();

		// Check if the address is valid.
		if ((address == IpAddress::NONE) || (address == IpAddress::BROADCAST))
		{
			return SOCKET_STATUS_ERROR;
		}

		// Bind the socket to the specified port.
		sockaddr_in addr = Socket::CreateAddress(address.ToInteger(), port);

		if (bind(GetHandle(), reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) == -1)
		{
			// Not likely to happen, but...
			Log::Error("Failed to bind listener socket to port %i\n", port);
			return SOCKET_STATUS_ERROR;
		}

		// Listen to the bound port.
		if (listen(GetHandle(), SOMAXCONN) == -1)
		{
			// Oops, socket is deaf.
			Log::Error("Failed to listen to port %i\n", port);
			return SOCKET_STATUS_ERROR;
		}

		return SOCKET_STATUS_DONE;
	}

	void TcpListener::Close()
	{
		// Simply close the socket.
		Socket::Close();
	}

	SocketStatus TcpListener::Accept(TcpSocket &socket)
	{
		// Make sure that we're listening.
		if (GetHandle() == Socket::InvalidSocketHandle())
		{
			Log::Error("Failed to accept a new connection, the socket is not listening\n");
			return SOCKET_STATUS_ERROR;
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

		return SOCKET_STATUS_DONE;
	}
}
