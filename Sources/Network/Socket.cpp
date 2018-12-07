#include "Socket.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif
#include <cstdio>
#include <cstring>
#include "Engine/Log.hpp"

namespace acid
{
	Socket::Socket(SocketType type) :
		m_type(type),
		m_socket(Socket::InvalidSocketHandle()),
		m_isBlocking(true)
	{
	}

	Socket::~Socket()
	{
		// Close the socket before it gets destructed.
		Close();
	}

	sockaddr_in Socket::CreateAddress(uint32_t address, unsigned short port)
	{
		sockaddr_in addr;
		std::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(address);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);

#if defined(ACID_BUILD_MACOS)
		addr.sin_len = sizeof(addr);
#endif

		return addr;
	}

	SocketHandle Socket::InvalidSocketHandle()
	{
#if defined(ACID_BUILD_WINDOWS)
		return INVALID_SOCKET;
#else
		return -1;
#endif
	}

	void Socket::CloseSocketHandle(SocketHandle sock)
	{
#if defined(ACID_BUILD_WINDOWS)
		closesocket(sock);
#else
		close(sock);
#endif
	}

	void Socket::SetHandleBlocking(SocketHandle sock, bool block)
	{
#if defined(ACID_BUILD_WINDOWS)
		u_long blocking = block ? 0 : 1;
		ioctlsocket(sock, FIONBIO, &blocking);
#else
		int32_t status = fcntl(sock, F_GETFL);

		if (block)
		{
			if (fcntl(sock, F_SETFL, status & ~O_NONBLOCK) == -1)
			{
				Log::Error("Failed to set file status flags: %i\n", errno);
			}
		}
		else
		{
			if (fcntl(sock, F_SETFL, status | O_NONBLOCK) == -1)
			{
				Log::Error("Failed to set file status flags: %i\n", errno);
			}
		}
#endif
	}

	SocketStatus Socket::GetErrorStatus()
	{
#if defined(ACID_BUILD_WINDOWS)
		switch (WSAGetLastError())
		{
			case WSAEWOULDBLOCK:
				return SOCKET_STATUS_NOT_READY;
			case WSAEALREADY:
				return SOCKET_STATUS_NOT_READY;
			case WSAECONNABORTED:
				return SOCKET_STATUS_DISCONNECTED;
			case WSAECONNRESET:
				return SOCKET_STATUS_DISCONNECTED;
			case WSAETIMEDOUT:
				return SOCKET_STATUS_DISCONNECTED;
			case WSAENETRESET:
				return SOCKET_STATUS_DISCONNECTED;
			case WSAENOTCONN:
				return SOCKET_STATUS_DISCONNECTED;
			case WSAEISCONN:
				return SOCKET_STATUS_DONE; // When connecting a non-blocking socket
			default:
				return SOCKET_STATUS_ERROR;
		}
#else
		// The followings are sometimes equal to EWOULDBLOCK,
		// so we have to make a special case for them in order
		// to avoid having double values in the switch case
		if ((errno == EAGAIN) || (errno == EINPROGRESS))
		{
			return SOCKET_STATUS_NOT_READY;
		}

		switch (errno)
		{
			case EWOULDBLOCK:
				return SOCKET_STATUS_NOT_READY;
			case ECONNABORTED:
				return SOCKET_STATUS_DISCONNECTED;
			case ECONNRESET:
				return SOCKET_STATUS_DISCONNECTED;
			case ETIMEDOUT:
				return SOCKET_STATUS_DISCONNECTED;
			case ENETRESET:
				return SOCKET_STATUS_DISCONNECTED;
			case ENOTCONN:
				return SOCKET_STATUS_DISCONNECTED;
			case EPIPE:
				return SOCKET_STATUS_DISCONNECTED;
			default:
				return SOCKET_STATUS_ERROR;
		}
#endif
	}

#if defined(ACID_BUILD_WINDOWS)
	// Windows needs some initialization and cleanup to get
	// sockets working properly... so let's create a class that will
	// do it automatically
	struct SocketInitializer
	{
		SocketInitializer()
		{
			WSADATA init;
			WSAStartup(MAKEWORD(2, 2), &init);
		}

		~SocketInitializer()
		{
			WSACleanup();
		}
	};

	SocketInitializer globalInitializer;
#endif

	bool Socket::IsBlocking() const
	{
		return m_isBlocking;
	}

	void Socket::SetBlocking(bool blocking)
	{
		// Apply if the socket is already created.
		if (m_socket != Socket::InvalidSocketHandle())
		{
			Socket::SetHandleBlocking(m_socket, blocking);
		}

		m_isBlocking = blocking;
	}

	SocketHandle Socket::GetHandle() const
	{
		return m_socket;
	}

	void Socket::Create()
	{
		// Don't create the socket if it already exists.
		if (m_socket == Socket::InvalidSocketHandle())
		{
			SocketHandle handle = socket(PF_INET, m_type == SOCKET_TYPE_TCP ? SOCK_STREAM : SOCK_DGRAM, 0);

			if (handle == Socket::InvalidSocketHandle())
			{
				Log::Error("Failed to create socket\n");
				return;
			}

			Create(handle);
		}
	}

	void Socket::Create(SocketHandle handle)
	{
		// Don't create the socket if it already exists.
		if (m_socket == Socket::InvalidSocketHandle())
		{
			// Assign the new handle.
			m_socket = handle;

			// Set the current blocking state.
			SetBlocking(m_isBlocking);

			if (m_type == SOCKET_TYPE_TCP)
			{
				// Disable the Nagle algorithm (i.e. removes buffering of TCP packets).
				int32_t yes = 1;

				if (setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1)
				{
					Log::Error("Failed to set socket option \"TCP_NODELAY\" ; all your TCP packets will be buffered\n");
				}

				// On Mac OS X, disable the SIGPIPE signal on disconnection.
#if defined(ACID_BUILD_MACOS)
				if (setsockopt(m_socket, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1)
				{
					Log::Error("Failed to set socket option \"SO_NOSIGPIPE\"\n");
				}
#endif
			}
			else
			{
				// Enable broadcast by default for UDP sockets.
				int32_t yes = 1;

				if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1)
				{
					Log::Error("Failed to enable broadcast on UDP socket\n");
				}
			}
		}
	}

	void Socket::Close()
	{
		// Close the socket.
		if (m_socket != Socket::InvalidSocketHandle())
		{
			Socket::CloseSocketHandle(m_socket);
			m_socket = Socket::InvalidSocketHandle();
		}
	}
}
