#pragma once

#include "StdAfx.hpp"

struct sockaddr_in;

namespace acid
{
// Define the low-level socket handle type, specific to each platform.
#if defined(ACID_BUILD_WINDOWS)
#if defined(_WIN64)
using SocketHandle = unsigned __int64;
#else
using SocketHandle = unsigned int;
#endif
using SocketAddrLength = int32_t;
#else
using SocketHandle = int32_t;
using SocketAddrLength = unsigned int;
#endif

/// <summary>
/// This class mainly defines internal stuff to be used by derived classes.
///
/// The only public features that it defines, and which is therefore common
/// to all the socket classes, is the blocking state.
/// All sockets can be set as blocking or non-blocking.
///
/// In blocking mode, socket functions will hang until the operation completes,
/// which means that the entire program (well, in fact the current thread if you use multiple ones)
/// will be stuck waiting for your socket operation to complete.
///
/// In non-blocking mode, all the socket functions will return immediately.
/// If the socket is not ready to complete the requested operation,
/// the function simply returns the proper status code (Socket::NotReady).
///
/// The default mode, which is blocking, is the one that is generally used,
/// in combination with threads or selectors. The non-blocking mode is rather used in real-time
/// applications that run an endless loop that can poll the socket often enough,
/// and cannot afford blocking this loop.
/// </summary>
class ACID_EXPORT Socket
{
public:
	/// <summary>
	/// Status codes that may be returned by socket functions.
	/// </summary>
	enum class Status
	{
		/// The socket has sent / received the data.
			Done, /// The socket is not ready to send / receive data yet.
			NotReady, /// The socket sent a part of the data.
			Partial, /// The TCP socket has been disconnected.
			Disconnected, /// An unexpected error happened.
			Error
	};

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
	static sockaddr_in CreateAddress(uint32_t address, uint16_t port);

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
	static Status GetErrorStatus();

	/// <summary>
	/// Tell whether the socket is in blocking or non-blocking mode.
	/// </summary>
	/// <returns> True if the socket is blocking, false otherwise. </returns>
	const bool &IsBlocking() const { return m_isBlocking; }

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
	/// Types of protocols that the socket can use.
	/// </summary>
	enum class Type
	{
		/// TCP protocol.
			Tcp, /// UDP protocol.
			Udp
	};

	/// <summary>
	/// Default constructor.
	/// This constructor can only be accessed by derived classes.
	/// </summary>
	/// <param name="type"> Type of the socket (TCP or UDP). </param>
	Socket(Type type);

	/// <summary>
	/// Return the internal handle of the socket.
	/// The returned handle may be invalid if the socket was not created yet (or already destroyed).
	/// This function can only be accessed by derived classes.
	/// </summary>
	/// <returns> The internal (OS-specific) handle of the socket. </returns>
	const SocketHandle &GetHandle() const { return m_socket; }

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

private:
	friend class SocketSelector;
	/// Type of the socket (TCP or UDP).
	Type m_type;
	/// Socket descriptor.
	SocketHandle m_socket;
	/// Current blocking mode of the socket.
	bool m_isBlocking;
};
}
