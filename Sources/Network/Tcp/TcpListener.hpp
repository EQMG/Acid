#pragma once

#include "Engine/Exports.hpp"
#include "Network/Socket.hpp"
#include "Network/IpAddress.hpp"

namespace acid
{
	class TcpSocket;

	/// <summary>
	/// Socket that listens to new TCP connections.
	/// </summary>
	class ACID_EXPORT TcpListener :
		public Socket
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		TcpListener();

		/// <summary>
		/// Get the port to which the socket is bound locally.
		/// If the socket is not listening to a port, this function returns 0.
		/// </summary>
		/// <returns> Port to which the socket is bound. </returns>
		unsigned short GetLocalPort() const;

		/// <summary>
		/// Start listening for incoming connection attempts.
		/// This function makes the socket start listening on the specified port, waiting for incoming connection attempts.
		/// If the socket is already listening on a port when this function is called,
		/// it will stop listening on the old port before starting to listen on the new port.
		/// </summary>
		/// <param name="port"> Port to listen on for incoming connection attempts. </param>
		/// <param name="address"> Address of the interface to listen on. </param>
		/// <returns> Status code. </returns>
		SocketStatus Listen(unsigned short port, const IpAddress &address = IpAddress::ANY);

		/// <summary>
		/// Stop listening and close the socket. This function gracefully stops the listener.
		/// If the socket is not listening, this function has no effect.
		/// </summary>
		void Close();

		/// <summary>
		/// Accept a new connection.
		/// If the socket is in blocking mode, this function will not return until a connection is actually received.
		/// </summary>
		/// <param name="socket"> Socket that will hold the new connection. </param>
		/// <returns> Status code. </returns>
		SocketStatus Accept(TcpSocket &socket);
	};
}
