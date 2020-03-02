#include "Socket.hpp"

#include <cstring>
#if defined(ACID_BUILD_WINDOWS)
#include <WinSock2.h>
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#endif

#include "Engine/Log.hpp"

namespace acid {
Socket::Socket(Type type) :
	type(type),
	socket(InvalidSocketHandle()) {
}

Socket::~Socket() {
	// Close the socket before it gets destructed.
	Close();
}

sockaddr_in Socket::CreateAddress(uint32_t address, uint16_t port) {
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

SocketHandle Socket::InvalidSocketHandle() {
#if defined(ACID_BUILD_WINDOWS)
	return INVALID_SOCKET;
#else
	return -1;
#endif
}

void Socket::CloseSocketHandle(SocketHandle sock) {
#if defined(ACID_BUILD_WINDOWS)
	closesocket(sock);
#else
	close(sock);
#endif
}

void Socket::SetHandleBlocking(SocketHandle sock, bool block) {
#if defined(ACID_BUILD_WINDOWS)
	u_long blocking = block ? 0 : 1;
	ioctlsocket(sock, FIONBIO, &blocking);
#else
	int32_t status = fcntl(sock, F_GETFL);

	if (block) {
		if (fcntl(sock, F_SETFL, status & ~O_NONBLOCK) == -1) {
			Log::Error("Failed to set file status flags: ", errno, '\n');
		}
	} else {
		if (fcntl(sock, F_SETFL, status | O_NONBLOCK) == -1) {
			Log::Error("Failed to set file status flags: ", errno, '\n');
		}
	}
#endif
}

Socket::Status Socket::GetErrorStatus() {
#if defined(ACID_BUILD_WINDOWS)
	switch (WSAGetLastError()) {
	case WSAEWOULDBLOCK:
		return Status::NotReady;
	case WSAEALREADY:
		return Status::NotReady;
	case WSAECONNABORTED:
		return Status::Disconnected;
	case WSAECONNRESET:
		return Status::Disconnected;
	case WSAETIMEDOUT:
		return Status::Disconnected;
	case WSAENETRESET:
		return Status::Disconnected;
	case WSAENOTCONN:
		return Status::Disconnected;
	case WSAEISCONN:
		return Status::Done; // When connecting a non-blocking socket
	default:
		return Status::Error;
	}
#else
	// The followings are sometimes equal to EWOULDBLOCK,
	// so we have to make a special case for them in order
	// to avoid having double values in the switch case
	if ((errno == EAGAIN) || (errno == EINPROGRESS)) {
		return Status::NotReady;
	}

	switch (errno) {
	case EWOULDBLOCK:
		return Status::NotReady;
	case ECONNABORTED:
		return Status::Disconnected;
	case ECONNRESET:
		return Status::Disconnected;
	case ETIMEDOUT:
		return Status::Disconnected;
	case ENETRESET:
		return Status::Disconnected;
	case ENOTCONN:
		return Status::Disconnected;
	case EPIPE:
		return Status::Disconnected;
	default:
		return Status::Error;
	}
#endif
}

#if defined(ACID_BUILD_WINDOWS)
// Windows needs some initialization and cleanup to get
// sockets working properly... so let's create a class that will
// do it automatically
struct SocketInitializer {
	SocketInitializer() {
		WSADATA init;
		WSAStartup(MAKEWORD(2, 2), &init);
	}

	~SocketInitializer() {
		WSACleanup();
	}
};

SocketInitializer globalInitializer;
#endif

void Socket::SetBlocking(bool blocking) {
	// Apply if the socket is already created.
	if (socket != InvalidSocketHandle()) {
		SetHandleBlocking(socket, blocking);
	}

	blocking = blocking;
}

void Socket::Create() {
	// Don't create the socket if it already exists.
	if (socket == InvalidSocketHandle()) {
		auto handle = ::socket(PF_INET, type == Type::Tcp ? SOCK_STREAM : SOCK_DGRAM, 0);

		if (handle == InvalidSocketHandle()) {
			Log::Error("Failed to create socket\n");
			return;
		}

		Create(handle);
	}
}

void Socket::Create(SocketHandle handle) {
	// Don't create the socket if it already exists.
	if (socket == InvalidSocketHandle()) {
		// Assign the new handle.
		socket = handle;

		// Set the current blocking state.
		SetBlocking(blocking);

		if (type == Type::Tcp) {
			// Disable the Nagle algorithm (i.e. removes buffering of TCP packets).
			int32_t yes = 1;

			if (setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1) {
				Log::Error("Failed to set socket option \"TCP_NODELAY\" ; all your TCP packets will be buffered\n");
			}

			// On Mac OS X, disable the SIGPIPE signal on disconnection.
#if defined(ACID_BUILD_MACOS)
			if (setsockopt(socket, SOL_SOCKET, SO_NOSIGPIPE, reinterpret_cast<char*>(&yes), sizeof(yes)) == -1) {
				Log::Error("Failed to set socket option \"SO_NOSIGPIPE\"\n");
			}
#endif
		} else {
			// Enable broadcast by default for UDP sockets.
			int32_t yes = 1;

			if (setsockopt(socket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char *>(&yes), sizeof(yes)) == -1) {
				Log::Error("Failed to enable broadcast on UDP socket\n");
			}
		}
	}
}

void Socket::Close() {
	// Close the socket.
	if (socket != InvalidSocketHandle()) {
		CloseSocketHandle(socket);
		socket = InvalidSocketHandle();
	}
}
}
