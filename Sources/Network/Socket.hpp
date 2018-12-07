#pragma once

#include <stdint.h>
#include <vector>
#include "Engine/Exports.hpp"

struct sockaddr_in;

namespace acid
{
// Define the low-level socket handle type, specific to each platform.
#if defined(ACID_BUILD_WINDOWS)
#if defined(_WIN64)
	typedef unsigned __int64 SocketHandle;
#else
	typedef unsigned int SocketHandle;
#endif
	typedef int32_t SocketAddrLength;
#else
	typedef int32_t SocketHandle;
	typedef unsigned int SocketAddrLength;
#endif

	/// <summary>
	/// Status codes that may be returned by socket functions.
	/// </summary>
	enum SocketStatus
	{
		/// The socket has sent / received the data.
		SOCKET_STATUS_DONE = 0,
		/// The socket is not ready to send / receive data yet.
		SOCKET_STATUS_NOT_READY = 1,
		/// The socket sent a part of the data.
		SOCKET_STATUS_PARTIAL = 2,
		/// The TCP socket has been disconnected.
		SOCKET_STATUS_DISCONNECTED = 3,
		/// An unexpected error happened.
		SOCKET_STATUS_ERROR = 4
	};

	/// <summary>
	/// Types of protocols that the socket can use.
	/// </summary>
	enum SocketType
	{
		/// TCP protocol.
		SOCKET_TYPE_TCP = 0,
		/// UDP protocol.
		SOCKET_TYPE_UDP = 1
	};

	/// <summary>
	/// Base class for all the socket types.
	/// </summary>
	class ACID_EXPORT Socket
	{
	private:
		friend class SocketSelector;

		/// Type of the socket (TCP or UDP).
		SocketType m_type;
		/// Socket descriptor.
		SocketHandle m_socket;
		/// Current blocking mode of the socket.
		bool m_isBlocking;
	public:
		/// <summary>
		/// Destructor that closes the socket.
		/// </summary>
		virtual ~Socket();

		/// <summary>
		/// Create an internal sockaddr_in address.
		/// </summary>
		/// <param name="address"> Target address. </param>
		/// <param name="port"> Target port. </param>
		/// <returns> sockaddr_in ready to be used by socket functions. </returns>
		static sockaddr_in CreateAddress(uint32_t address, unsigned short port);

		/// <summary>
		/// Return the value of the invalid socket.
		/// </summary>
		/// <returns> Special value of the invalid socket. </returns>
		static SocketHandle InvalidSocketHandle();

		/// <summary>
		/// Close and destroy a socket.
		/// </summary>
		/// <param name="sock"> Handle of the socket to close. </param>
		static void CloseSocketHandle(SocketHandle sock);

		/// <summary>
		/// Set a socket as blocking or non-blocking.
		/// </summary>
		/// <param name="sock"> Handle of the socket. </param>
		/// <param name="block"> New blocking state of the socket. </param>
		static void SetHandleBlocking(SocketHandle sock, bool block);

		/// <summary>
		/// Get the last socket error status.
		/// </summary>
		/// <returns> Status corresponding to the last socket error. </returns>
		static SocketStatus GetErrorStatus();

		/// <summary>
		/// Tell whether the socket is in blocking or non-blocking mode.
		/// </summary>
		/// <returns> True if the socket is blocking, false otherwise. </returns>
		bool IsBlocking() const;

		/// <summary>
		/// Set the blocking state of the socket.
		/// In blocking mode, calls will not return until they have completed their task.
		/// For example, a call to Receive in blocking mode won't return until some data was actually received.
		/// In non-blocking mode, calls will always return immediately, using the return code to signal
		/// whether there was data available or not. By default, all sockets are blocking.
		/// </summary>
		/// <param name="blocking"> True to set the socket as blocking, false for non-blocking. </param>
		void SetBlocking(bool blocking);
	protected:
		/// <summary>
		/// Default constructor.
		/// This constructor can only be accessed by derived classes.
		/// </summary>
		/// <param name="type"> Type of the socket (TCP or UDP). </param>
		Socket(SocketType type);

		/// <summary>
		/// Return the internal handle of the socket.
		/// The returned handle may be invalid if the socket was not created yet (or already destroyed).
		/// This function can only be accessed by derived classes.
		/// </summary>
		/// <returns> The internal (OS-specific) handle of the socket. </returns>
		SocketHandle GetHandle() const;

		/// <summary>
		/// Create the internal representation of the socket.
		/// This function can only be accessed by derived classes.
		/// </summary>
		void Create();

		/// <summary>
		/// Create the internal representation of the socket from a socket handle.
		/// This function can only be accessed by derived classes.
		/// </summary>
		/// <param name="handle"> OS-specific handle of the socket to wrap. </param>
		void Create(SocketHandle handle);

		/// <summary>
		/// Close the socket gracefully.
		/// This function can only be accessed by derived classes.
		/// </summary>
		void Close();
	};
}
