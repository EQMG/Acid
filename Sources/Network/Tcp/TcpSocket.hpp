#pragma once

#include <stdint.h>
#include "Engine/Exports.hpp"
#include "Maths/Time.hpp"
#include "Network/Socket.hpp"

namespace acid
{
	class IpAddress;

	class Packet;

	///
	/// \brief Structure holding the data of a pending packet.
	///
	class ACID_EXPORT PendingPacket
	{
	public:
		PendingPacket() :
			m_size(0),
			m_sizeReceived(0),
			m_data(std::vector<char>())
		{
		}

		uint32_t m_size; /// Data of packet size.
		std::size_t m_sizeReceived; /// Number of size bytes received so far.
		std::vector<char> m_data; /// Data of the packet.
	};

	///
	/// \brief Specialized socket using the TCP protocol
	///
	class ACID_EXPORT TcpSocket :
		public Socket
	{
	private:
		friend class TcpListener;

		PendingPacket m_pendingPacket; /// Temporary data of the packet currently being received.
	public:
		///
		/// \brief Default constructor
		///
		TcpSocket();

		///
		/// \brief Get the port to which the socket is bound locally
		///
		/// If the socket is not connected, this function returns 0.
		///
		/// \return Port to which the socket is bound
		///
		/// \see connect, getRemotePort
		///
		unsigned short GetLocalPort() const;

		///
		/// \brief Get the address of the connected peer
		///
		/// It the socket is not connected, this function returns
		/// IpAddress::None.
		///
		/// \return Address of the remote peer
		///
		/// \see getRemotePort
		///
		IpAddress GetRemoteAddress() const;

		///
		/// \brief Get the port of the connected peer to which
		///        the socket is connected
		///
		/// If the socket is not connected, this function returns 0.
		///
		/// \return Remote port to which the socket is connected
		///
		/// \see getRemoteAddress
		///
		unsigned short GetRemotePort() const;

		///
		/// \brief Connect the socket to a remote peer
		///
		/// In blocking mode, this function may take a while, especially
		/// if the remote peer is not reachable. The last parameter allows
		/// you to stop trying to connect after a given timeout.
		/// If the socket is already connected, the connection is
		/// forcibly disconnected before attempting to connect again.
		///
		/// \param remoteAddress Address of the remote peer
		/// \param remotePort    Port of the remote peer
		/// \param timeout       Optional maximum time to wait
		///
		/// \return Status code
		///
		/// \see disconnect
		///
		SocketStatus Connect(const IpAddress &remoteAddress, unsigned short remotePort, Time timeout = Time::ZERO);

		///
		/// \brief Disconnect the socket from its remote peer
		///
		/// This function gracefully closes the connection. If the
		/// socket is not connected, this function has no effect.
		///
		/// \see connect
		///
		void Disconnect();

		///
		/// \brief Send raw data to the remote peer
		///
		/// To be able to handle partial sends over non-blocking
		/// sockets, use the send(const void*, std::size_t, std::size_t&)
		/// overload instead.
		/// This function will fail if the socket is not connected.
		///
		/// \param data Pointer to the sequence of bytes to send
		/// \param size Number of bytes to send
		///
		/// \return Status code
		///
		/// \see receive
		///
		SocketStatus Send(const void *data, std::size_t size);

		///
		/// \brief Send raw data to the remote peer
		///
		/// This function will fail if the socket is not connected.
		///
		/// \param data Pointer to the sequence of bytes to send
		/// \param size Number of bytes to send
		/// \param sent The number of bytes sent will be written here
		///
		/// \return Status code
		///
		/// \see receive
		///
		SocketStatus Send(const void *data, std::size_t size, std::size_t &sent);

		///
		/// \brief Receive raw data from the remote peer
		///
		/// In blocking mode, this function will wait until some
		/// bytes are actually received.
		/// This function will fail if the socket is not connected.
		///
		/// \param data     Pointer to the array to fill with the received bytes
		/// \param size     Maximum number of bytes that can be received
		/// \param received This variable is filled with the actual number of bytes received
		///
		/// \return Status code
		///
		/// \see send
		///
		SocketStatus Receive(void *data, std::size_t size, std::size_t &received);

		///
		/// \brief Send a formatted packet of data to the remote peer
		///
		/// In non-blocking mode, if this function returns Socket::Partial,
		/// you \em must retry sending the same unmodified packet before sending
		/// anything else in order to guarantee the packet arrives at the remote
		/// peer uncorrupted.
		/// This function will fail if the socket is not connected.
		///
		/// \param packet Packet to send
		///
		/// \return Status code
		///
		/// \see receive
		///
		SocketStatus Send(Packet &packet);

		///
		/// \brief Receive a formatted packet of data from the remote peer
		///
		/// In blocking mode, this function will wait until the whole packet
		/// has been received.
		/// This function will fail if the socket is not connected.
		///
		/// \param packet Packet to fill with the received data
		///
		/// \return Status code
		///
		/// \see send
		///
		SocketStatus Receive(Packet &packet);
	};
}
