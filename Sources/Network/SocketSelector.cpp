#include "SocketSelector.hpp"

#if defined(ACID_BUILD_WINDOWS)
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif
#include "Engine/Log.hpp"
#include "Socket.hpp"

#ifdef _MSC_VER
#pragma warning(disable : 4127) // "conditional expression is constant" generated by the FD_SET macro
#endif

namespace acid
{
struct SocketSelector::SocketSelectorImpl
{
	/// Set
	/// containing
	/// all
	/// the
	/// sockets
	/// handles.
	fd_set allSockets;
	/// Set
	/// containing
	/// handles
	/// of
	/// the
	/// sockets
	/// that
	/// are
	/// ready.
	fd_set socketsReady;
	/// Maximum
	/// socket
	/// handle.
	int maxSocket;
	/// Number
	/// of
	/// socket
	/// handles.
	int socketCount;
};

SocketSelector::SocketSelector() : m_impl(std::make_unique<SocketSelectorImpl>())
{
	Clear();
}

SocketSelector::SocketSelector(const SocketSelector& copy) : m_impl(std::make_unique<SocketSelectorImpl>(*copy.m_impl)) {}

void SocketSelector::Add(Socket& socket)
{
	SocketHandle handle = socket.GetHandle();

	if(handle != Socket::InvalidSocketHandle())
		{
#if defined(ACID_BUILD_WINDOWS)
			if(m_impl->socketCount >= FD_SETSIZE)
				{
					Log::Error("The socket can't be added to the selector because the selector is full. This is a limitation of your operating system's FD_SETSIZE setting.\n");
					return;
				}

			if(FD_ISSET(handle, &m_impl->allSockets))
				{
					return;
				}

			m_impl->socketCount++;
#else
			if(handle >= FD_SETSIZE)
				{
					Log::Error("The socket can't be added to the selector because its ID is too high. This is a limitation of your operating system's FD_SETSIZE setting.\n");
					return;
				}

			// SocketHandle is an int in POSIX
			m_impl->maxSocket = std::max(m_impl->maxSocket, handle);
#endif

			FD_SET(handle, &m_impl->allSockets);
		}
}

void SocketSelector::Remove(Socket& socket)
{
	SocketHandle handle = socket.GetHandle();

	if(handle != Socket::InvalidSocketHandle())
		{
#if defined(ACID_BUILD_WINDOWS)
			if(!FD_ISSET(handle, &m_impl->allSockets))
				{
					return;
				}

			m_impl->socketCount--;

#else
			if(handle >= FD_SETSIZE)
				{
					return;
				}
#endif

			FD_CLR(handle, &m_impl->allSockets);
			FD_CLR(handle, &m_impl->socketsReady);
		}
}

void SocketSelector::Clear()
{
	FD_ZERO(&m_impl->allSockets);
	FD_ZERO(&m_impl->socketsReady);

	m_impl->maxSocket = 0;
	m_impl->socketCount = 0;
}

bool SocketSelector::Wait(const Time timeout)
{
	// Setup
	// the
	// timeout
	timeval time = {};
	time.tv_sec = static_cast<long>(timeout.AsMicroseconds() / 1000000);
	time.tv_usec = static_cast<long>(timeout.AsMicroseconds() % 1000000);

	// Initialize
	// the
	// set
	// that
	// will
	// contain
	// the
	// sockets
	// that
	// are
	// ready
	m_impl->socketsReady = m_impl->allSockets;

	// Wait
	// until
	// one
	// of
	// the
	// sockets
	// is
	// ready
	// for
	// reading,
	// or
	// timeout
	// is
	// reached
	// The
	// first
	// parameter
	// is
	// ignored
	// on
	// Windows
	int count = select(m_impl->maxSocket + 1, &m_impl->socketsReady, nullptr, nullptr, timeout != Time::Zero ? &time : nullptr);

	return count > 0;
}

bool SocketSelector::IsReady(const Socket& socket) const
{
	SocketHandle handle = socket.GetHandle();

	if(handle != Socket::InvalidSocketHandle())
		{
#if !defined(ACID_BUILD_WINDOWS)
			if(handle >= FD_SETSIZE)
				{
					return false;
				}
#endif

			return FD_ISSET(handle, &m_impl->socketsReady) != 0;
		}

	return false;
}

SocketSelector& SocketSelector::operator=(const SocketSelector& right)
{
	SocketSelector temp(right);
	m_impl.swap(temp.m_impl);
	return *this;
}
}
