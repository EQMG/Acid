#pragma once

#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"

namespace acid
{
	class Socket;

	/// <summary>
	/// Multiplexer that allows to read from multiple sockets.
	/// </summary>
	class ACID_EXPORT SocketSelector
	{
	private:
		struct SocketSelectorImpl;

		/// Opaque pointer to the implementation (which requires OS-specific types).
		SocketSelectorImpl *m_impl;
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		SocketSelector();

		/// <summary>
		/// Copy constructor.
		/// </summary>
		/// <param name="copy"> Instance to copy. </param>
		SocketSelector(const SocketSelector &copy);

		/// <summary>
		/// Destructor.
		/// </summary>
		~SocketSelector();

		/// <summary>
		/// Add a new socket to the selector.
		///
		/// This function keeps a weak reference to the socket, so you have to make sure
		/// that the socket is not destroyed while it is stored in the selector.
		/// This function does nothing if the socket is not valid.
		/// </summary>
		/// <param name="socket"> Reference to the socket to add. </param>
		void Add(Socket &socket);

		/// <summary>
		/// Remove a socket from the selector.
		/// This function doesn't destroy the socket, it simply removes the reference that the selector has to it.
		/// </summary>
		/// <param name="socket"> Reference to the socket to remove. </param>
		void Remove(Socket &socket);

		/// <summary>
		/// Remove all the sockets stored in the selector.
		/// This function doesn't destroy any instance, it simply removes all the references that
		/// the selector has to external sockets.
		/// </summary>
		void Clear();

		/// <summary>
		/// Wait until one or more sockets are ready to receive.
		/// This function returns as soon as at least one socket has some data available to be received.
		/// To know which sockets are ready, use the isReady function.
		/// If you use a timeout and no socket is ready before the timeout is over, the function returns false.
		/// </summary>
		/// <param name="timeout"> Maximum time to wait, (use Time::Zero for infinity). </param>
		/// <returns> True if there are sockets ready, false otherwise. </returns>
		bool Wait(Time timeout = Time::ZERO);

		/// <summary>
		/// Test a socket to know if it is ready to receive data.
		///
		/// This function must be used after a call to Wait, to know which sockets are ready to receive data.
		/// If a socket is ready, a call to receive will never block because we know that there is data available to read.
		/// Note that if this function returns true for a TcpListener, this means that it is ready to accept a new connection.
		/// </summary>
		/// <param name="socket"> Socket to test. </param>
		/// <returns> True if the socket is ready to read, false otherwise. </returns>
		bool IsReady(Socket &socket) const;

		/// <summary>
		/// Overload of assignment operator.
		/// </summary>
		/// <param name="right"> Instance to assign. </param>
		/// <returns> Reference to self. </returns>
		SocketSelector &operator=(const SocketSelector &right);
	};
}
