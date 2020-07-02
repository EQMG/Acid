#pragma once

#include <cstdint>

#include "Export.hpp"

struct sockaddr_in;

namespace acid {
// Define the low-level socket handle type, specific to each platform.
#ifdef ACID_BUILD_WINDOWS
#ifdef _WIN64
using SocketHandle = unsigned __int64;
#else
using SocketHandle = unsigned int;
#endif
using SocketAddrLength = int32_t;
#else
using SocketHandle = int32_t;
using SocketAddrLength = unsigned int;
#endif

/**
 * @brief This class mainly defines internal stuff to be used by derived classes.
 *
 * The only public features that it defines, and which is therefore common
 * to all the socket classes, is the blocking state.
 * All sockets can be set as blocking or non-blocking.
 *
 * In blocking mode, socket functions will hang until the operation completes,
 * which means that the entire program (well, in fact the current thread if you use multiple ones)
 * will be stuck waiting for your socket operation to complete.
 *
 * In non-blocking mode, all the socket functions will return immediately.
 * If the socket is not ready to complete the requested operation,
 * the function simply returns the proper status code (Socket::NotReady).
 *
 * The default mode, which is blocking, is the one that is generally used,
 * in combination with threads or selectors. The non-blocking mode is rather used in real-time
 * applications that run an endless loop that can poll the socket often enough,
 * and cannot afford blocking this loop.
 */
class ACID_EXPORT Socket {
	friend class SocketSelector;
public:
	/**
	 * @brief Status codes that may be returned by socket functions.
	 */
	enum class Status {
		/// The socket has sent / received the data.
		Done,
		/// The socket is not ready to send / receive data yet.
		NotReady,
		/// The socket sent a part of the data.
		Partial,
		/// The TCP socket has been disconnected.
		Disconnected,
		/// An unexpected error happened.
		Error
	};

	/**
	 * Destructor that closes the socket.
	 */
	virtual ~Socket();

	/**
	 * Create an internal sockaddr_in address.
	 * @param address Target address.
	 * @param port Target port.
	 * @return sockaddr_in ready to be used by socket functions.
	 */
	static sockaddr_in CreateAddress(uint32_t address, uint16_t port);

	/**
	 * Return the value of the invalid socket.
	 * @return Special value of the invalid socket.
	 */
	static SocketHandle InvalidSocketHandle();

	/**
	 * Close and destroy a socket.
	 * @param sock Handle of the socket to close.
	 */
	static void CloseSocketHandle(SocketHandle sock);

	/**
	 * Set a socket as blocking or non-blocking.
	 * @param sock Handle of the socket.
	 * @param block New blocking state of the socket.
	 */
	static void SetHandleBlocking(SocketHandle sock, bool block);

	/**
	 * Get the last socket error status.
	 * @return Status corresponding to the last socket error.
	 */
	static Status GetErrorStatus();

	/**
	 * Tell whether the socket is in blocking or non-blocking mode.
	 * @return True if the socket is blocking, false otherwise.
	 */
	bool IsBlocking() const { return blocking; }

	/**
	 * Set the blocking state of the socket.
	 * In blocking mode, calls will not return until they have completed their task.
	 * For example, a call to Receive in blocking mode won't return until some data was actually received.
	 * In non-blocking mode, calls will always return immediately, using the return code to signal
	 * whether there was data available or not. By default, all sockets are blocking.
	 * @param blocking True to set the socket as blocking, false for non-blocking.
	 */
	void SetBlocking(bool blocking);

protected:
	/**
	 * Types of protocols that the socket can use.
	 */
	enum class Type {
		/// TCP protocol.
		Tcp,
		/// UDP protocol.
		Udp
	};

	/**
	 * Default constructor.
	 * This constructor can only be accessed by derived classes.
	 * @param type Type of the socket (TCP or UDP).
	 */
	Socket(Type type);

	/**
	 * Return the internal handle of the socket.
	 * The returned handle may be invalid if the socket was not created yet (or already destroyed).
	 * This function can only be accessed by derived classes.
	 * @return The internal (OS-specific) handle of the socket.
	 */
	const SocketHandle &GetHandle() const { return socket; }

	/**
	 * Create the internal representation of the socket.
	 * This function can only be accessed by derived classes.
	 */
	void Create();

	/**
	 * Create the internal representation of the socket from a socket handle.
	 * This function can only be accessed by derived classes.
	 * @param handle OS-specific handle of the socket to wrap.
	 */
	void Create(SocketHandle handle);

	/**
	 * Close the socket gracefully.
	 * This function can only be accessed by derived classes.
	 */
	void Close();

private:
	/// Type of the socket (TCP or UDP).
	Type type;
	/// Socket descriptor.
	SocketHandle socket;
	/// Current blocking mode of the socket.
	bool blocking = true;
};
}
