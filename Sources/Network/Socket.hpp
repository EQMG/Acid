#pragma once

#if defined(ACID_BUILD_WINDOWS)
#ifdef _WIN32_WINDOWS
#undef _WIN32_WINDOWS
#endif
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINDOWS 0x0501
#define _WIN32_WINNT   0x0501
#include <winsock2.h>
#include <ws2tcpip.h>
#include <basetsd.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <stdint.h>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
// Define the low-level socket handle type, specific to each platform.
#if defined(ACID_BUILD_WINDOWS)
	typedef UINT_PTR SocketHandle;
#else
	typedef int SocketHandle;
#endif

	///
	/// \brief Status codes that may be returned by socket functions.
	///
	enum SocketStatus
	{
		SOCKET_STATUS_DONE = 0, /// The socket has sent / received the data.
		SOCKET_STATUS_NOT_READY = 1, /// The socket is not ready to send / receive data yet.
		SOCKET_STATUS_PARTIAL = 2, /// The socket sent a part of the data.
		SOCKET_STATUS_DISCONNECTED = 3, /// The TCP socket has been disconnected.
		SOCKET_STATUS_ERROR = 4 /// An unexpected error happened.
	};

	///
	/// \brief Types of protocols that the socket can use.
	///
	enum SocketType
	{
		SOCKET_TYPE_TCP = 0, /// TCP protocol.
		SOCKET_TYPE_UDP = 1  /// UDP protocol.
	};

	///
	/// \brief Base class for all the socket types.
	///
	class ACID_EXPORT Socket
	{
	private:
		SocketType m_type; /// Type of the socket (TCP or UDP).
		SocketHandle m_socket; /// Socket descriptor.
		bool m_isBlocking; /// Current blocking mode of the socket.
	public:
#if (defined ACID_BUILD_WINDOWS)
		typedef int AddrLength;
#else
		typedef socklen_t AddrLength;
#endif

		///
		/// \brief Destructor
		///
		virtual ~Socket();

		///
		/// \brief Create an internal sockaddr_in address
		///
		/// \param address Target address
		/// \param port    Target port
		///
		/// \return sockaddr_in ready to be used by socket functions
		///
		static sockaddr_in CreateAddress(uint32_t address, unsigned short port);

		///
		/// \brief Return the value of the invalid socket
		///
		/// \return Special value of the invalid socket
		///
		static SocketHandle InvalidSocketHandle();

		///
		/// \brief Close and destroy a socket
		///
		/// \param sock Handle of the socket to close
		///
		static void CloseSocketHandle(SocketHandle sock);

		///
		/// \brief Set a socket as blocking or non-blocking
		///
		/// \param sock  Handle of the socket
		/// \param block New blocking state of the socket
		///
		static void SetHandleBlocking(SocketHandle sock, bool block);

		///
		/// Get the last socket error status
		///
		/// \return Status corresponding to the last socket error
		///
		static SocketStatus GetErrorStatus();

		///
		/// \brief Tell whether the socket is in blocking or non-blocking mode
		///
		/// \return True if the socket is blocking, false otherwise
		///
		/// \see setBlocking
		///
		bool IsBlocking() const;

		///
		/// \brief Set the blocking state of the socket
		///
		/// In blocking mode, calls will not return until they have
		/// completed their task. For example, a call to Receive in
		/// blocking mode won't return until some data was actually
		/// received.
		/// In non-blocking mode, calls will always return immediately,
		/// using the return code to signal whether there was data
		/// available or not.
		/// By default, all sockets are blocking.
		///
		/// \param blocking True to set the socket as blocking, false for non-blocking
		///
		/// \see isBlocking
		///
		void SetBlocking(bool blocking);

	protected:
		///
		/// \brief Default constructor.
		///
		/// This constructor can only be accessed by derived classes.
		///
		/// \param type Type of the socket (TCP or UDP).
		///
		Socket(SocketType type);

		///
		/// \brief Return the internal handle of the socket.
		///
		/// The returned handle may be invalid if the socket
		/// was not created yet (or already destroyed).
		/// This function can only be accessed by derived classes.
		///
		/// \return The internal (OS-specific) handle of the socket
		///
		SocketHandle GetHandle() const;

		///
		/// \brief Create the internal representation of the socket
		///
		/// This function can only be accessed by derived classes.
		///
		void Create();

		///
		/// \brief Create the internal representation of the socket
		///        from a socket handle.
		///
		/// This function can only be accessed by derived classes.
		///
		/// \param handle OS-specific handle of the socket to wrap.
		///
		void Create(SocketHandle handle);

		///
		/// \brief Close the socket gracefully.
		///
		/// This function can only be accessed by derived classes.
		///
		void Close();
	};
}
