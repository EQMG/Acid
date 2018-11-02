#pragma once

#include <stdint.h>
#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"
#include "Network/Socket.hpp"

namespace acid
{
	class IpAddress;

	class Packet;

	/// <summary>
	/// Structure holding the data of a pending packet.
	/// </summary>
	class ACID_EXPORT PendingPacket
	{
	public:
		PendingPacket() :
			m_size(0),
			m_sizeReceived(0),
			m_data(std::vector<char>())
		{
		}

		/// Data of packet size.
		uint32_t m_size;
		/// Number of size bytes received so far.
		std::size_t m_sizeReceived;
		/// Data of the packet.
		std::vector<char> m_data;
	};

	/// <summary>
	/// Specialized socket using the TCP protocol
	/// </summary>
	class ACID_EXPORT TcpSocket :
		public Socket
	{
	private:
		friend class TcpListener;

		/// Temporary data of the packet currently being received.
		PendingPacket m_pendingPacket;
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		TcpSocket();

		/// <summary>
		/// Get the port to which the socket is bound locally. If the socket is not connected, this function returns 0.
		/// </summary>
		/// <returns> Port to which the socket is bound. </returns>
		unsigned short GetLocalPort() const;

		/// <summary>
		/// Get the address of the connected peer.
		/// It the socket is not connected, this function returns IpAddress::NONE.
		/// </summary>
		/// <returns> Address of the remote peer. </returns>
		IpAddress GetRemoteAddress() const;

		/// <summary>
		/// Get the port of the connected peer to which the socket is connected.
		/// If the socket is not connected, this function returns 0.
		/// </summary>
		/// <returns> Remote port to which the socket is connected. </returns>
		unsigned short GetRemotePort() const;

		/// <summary>
		/// Connect the socket to a remote peer.
		/// In blocking mode, this function may take a while, especially if the remote peer is not reachable.
		/// The last parameter allows you to stop trying to connect after a given timeout.
		/// If the socket is already connected, the connection is forcibly disconnected before attempting to connect again.
		/// </summary>
		/// <param name="remoteAddress"> Address of the remote peer. </param>
		/// <param name="remotePort"> Port of the remote peer. </param>
		/// <param name="timeout"> Optional maximum time to wait. </param>
		/// <returns> Status code. </returns>
		SocketStatus Connect(const IpAddress &remoteAddress, unsigned short remotePort, Time timeout = Time::ZERO);

		/// <summary>
		/// Disconnect the socket from its remote peer.
		/// This function gracefully closes the connection. If the socket is not connected, this function has no effect.
		/// </summary>
		void Disconnect();

		/// <summary>
		/// Send raw data to the remote peer.
		/// To be able to handle partial sends over non-blocking sockets,
		/// use the send(const void*, std::size_t, std::size_t&) overload instead.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="data"> Pointer to the sequence of bytes to send. </param>
		/// <param name="size"> Number of bytes to send. </param>
		/// <returns> Status code. </returns>
		SocketStatus Send(const void *data, std::size_t size);

		/// <summary>
		/// Send raw data to the remote peer. This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="data"> Pointer to the sequence of bytes to send. </param>
		/// <param name="size"> Number of bytes to send. </param>
		/// <param name="sent"> The number of bytes sent will be written here. </param>
		/// <returns> Status code. </returns>
		SocketStatus Send(const void *data, std::size_t size, std::size_t &sent);

		/// <summary>
		/// Receive raw data from the remote peer.
		/// In blocking mode, this function will wait until some bytes are actually received.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="data"> Pointer to the array to fill with the received bytes. </param>
		/// <param name="size"> Maximum number of bytes that can be received. </param>
		/// <param name="received"> This variable is filled with the actual number of bytes received. </param>
		/// <returns> Status code. </returns>
		SocketStatus Receive(void *data, std::size_t size, std::size_t &received);

		/// <summary>
		/// Send a formatted packet of data to the remote peer.
		/// In non-blocking mode, if this function returns SOCKET_STATUS_PARTIAL, you \em must retry sending the same unmodified
		/// packet before sending anything else in order to guarantee the packet arrives at the remote peer uncorrupted.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="packet"> Packet to send. </param>
		/// <returns> Status code. </returns>
		SocketStatus Send(Packet &packet);

		/// <summary>
		/// Receive a formatted packet of data from the remote peer.
		/// In blocking mode, this function will wait until the whole packet has been received.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="packet"> Packet to fill with the received data. </param>
		/// <returns> Status code. </returns>
		SocketStatus Receive(Packet &packet);
	};
}
