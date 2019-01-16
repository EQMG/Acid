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
	/// TCP is a connected protocol, which means that a TCP socket can only communicate with
	/// the host it is connected to. It can't send or receive anything if it is not connected.
	///
	/// The TCP protocol is reliable but adds a slight overhead. It ensures that your data
	/// will always be received in order and without errors (no data corrupted, lost or duplicated).
	///
	/// When a socket is connected to a remote host, you can retrieve informations about this host
	/// with the GetRemoteAddress and getRemotePort functions. You can also get the local port to
	/// which the socket is bound (which is automatically chosen when the socket is connected),
	/// with the GetLocalPort function.
	///
	/// Sending and receiving data can use either the low-level or the high-level functions.
	/// The low-level functions process a raw sequence of bytes, and cannot ensure that one call to
	/// Send will exactly match one call to Receive at the other end of the socket.
	///
	/// The high-level interface uses packets (see acid::Packet), which are easier to use and
	/// provide more safety regarding the data that is exchanged.
	/// You can look at the acid::Packet class to get more details about how they work.
	///
	/// The socket is automatically disconnected when it is destroyed, but if you want to
	/// explicitly close the connection while the socket instance is still alive, you can call disconnect.
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
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		TcpSocket();

		/// <summary>
		/// Get the port to which the socket is bound locally. If the socket is not connected, this function returns 0.
		/// </summary>
		/// <returns> Port to which the socket is bound. </returns>
		uint16_t GetLocalPort() const;

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
		uint16_t GetRemotePort() const;

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
		Socket::Status Connect(const IpAddress &remoteAddress, const uint16_t &remotePort, const Time &timeout = Time::Zero);

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
		Socket::Status Send(const void *data, const std::size_t &size);

		/// <summary>
		/// Send raw data to the remote peer. This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="data"> Pointer to the sequence of bytes to send. </param>
		/// <param name="size"> Number of bytes to send. </param>
		/// <param name="sent"> The number of bytes sent will be written here. </param>
		/// <returns> Status code. </returns>
		Socket::Status Send(const void *data, const std::size_t &size, std::size_t &sent);

		/// <summary>
		/// Receive raw data from the remote peer.
		/// In blocking mode, this function will wait until some bytes are actually received.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="data"> Pointer to the array to fill with the received bytes. </param>
		/// <param name="size"> Maximum number of bytes that can be received. </param>
		/// <param name="received"> This variable is filled with the actual number of bytes received. </param>
		/// <returns> Status code. </returns>
		Socket::Status Receive(void *data, const std::size_t &size, std::size_t &received);

		/// <summary>
		/// Send a formatted packet of data to the remote peer.
		/// In non-blocking mode, if this function returns SOCKET_STATUS_PARTIAL, you \em must retry sending the same unmodified
		/// packet before sending anything else in order to guarantee the packet arrives at the remote peer uncorrupted.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="packet"> Packet to send. </param>
		/// <returns> Status code. </returns>
		Socket::Status Send(Packet &packet);

		/// <summary>
		/// Receive a formatted packet of data from the remote peer.
		/// In blocking mode, this function will wait until the whole packet has been received.
		/// This function will fail if the socket is not connected.
		/// </summary>
		/// <param name="packet"> Packet to fill with the received data. </param>
		/// <returns> Status code. </returns>
		Socket::Status Receive(Packet &packet);
	private:
		friend class TcpListener;
		/// Temporary data of the packet currently being received.
		PendingPacket m_pendingPacket;
	};
}
