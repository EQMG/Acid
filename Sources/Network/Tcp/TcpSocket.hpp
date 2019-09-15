#pragma once

#include "Maths/Time.hpp"
#include "Network/Socket.hpp"

namespace acid {
class IpAddress;
class Packet;

/**
 * @brief TCP is a connected protocol, which means that a TCP socket can only communicate with
 * the host it is connected to. It can't send or receive anything if it is not connected.
 *
 * The TCP protocol is reliable but adds a slight overhead. It ensures that your data
 * will always be received in order and without errors (no data corrupted, lost or duplicated).
 *
 * When a socket is connected to a remote host, you can retrieve informations about this host
 * with the GetRemoteAddress and getRemotePort functions. You can also get the local port to
 * which the socket is bound (which is automatically chosen when the socket is connected),
 * with the GetLocalPort function.
 *
 * Sending and receiving data can use either the low-level or the high-level functions.
 * The low-level functions process a raw sequence of bytes, and cannot ensure that one call to
 * Send will exactly match one call to Receive at the other end of the socket.
 *
 * The high-level interface uses packets (see acid::Packet), which are easier to use and
 * provide more safety regarding the data that is exchanged.
 * You can look at the acid::Packet class to get more details about how they work.
 *
 * The socket is automatically disconnected when it is destroyed, but if you want to
 * explicitly close the connection while the socket instance is still alive, you can call disconnect.
 **/
class ACID_EXPORT PendingPacket {
public:
	PendingPacket() = default;

	/// Data of packet size.
	uint32_t m_size = 0;
	/// Number of size bytes received so far.
	std::size_t m_sizeReceived = 0;
	/// Data of the packet.
	std::vector<char> m_data;
};

/**
 * @brief Specialized socket using the TCP protocol
 **/
class ACID_EXPORT TcpSocket : public Socket {
	friend class TcpListener;
public:
	/**
	 * Default constructor.
	 **/
	TcpSocket();

	/**
	 * Get the port to which the socket is bound locally. If the socket is not connected, this function returns 0.
	 * @return Port to which the socket is bound.
	 **/
	uint16_t GetLocalPort() const;

	/**
	 * Get the address of the connected peer.
	 * It the socket is not connected, this function returns IpAddress::NONE.
	 * @return Address of the remote peer.
	 **/
	IpAddress GetRemoteAddress() const;

	/**
	 * Get the port of the connected peer to which the socket is connected.
	 * If the socket is not connected, this function returns 0.
	 * @return Remote port to which the socket is connected.
	 **/
	uint16_t GetRemotePort() const;

	/**
	 * Connect the socket to a remote peer.
	 * In blocking mode, this function may take a while, especially if the remote peer is not reachable.
	 * The last parameter allows you to stop trying to connect after a given timeout.
	 * If the socket is already connected, the connection is forcibly disconnected before attempting to connect again.
	 * @param remoteAddress Address of the remote peer.
	 * @param remotePort Port of the remote peer.
	 * @param timeout Optional maximum time to wait.
	 * @return Status code.
	 **/
	Status Connect(const IpAddress &remoteAddress, uint16_t remotePort, const Time &timeout = 0s);

	/**
	 * Disconnect the socket from its remote peer.
	 * This function gracefully closes the connection. If the socket is not connected, this function has no effect.
	 **/
	void Disconnect();

	/**
	 * Send raw data to the remote peer.
	 * To be able to handle partial sends over non-blocking sockets,
	 * use the send(const void*, std::size_t, std::size_t&) overload instead.
	 * This function will fail if the socket is not connected.
	 * @param data Pointer to the sequence of bytes to send.
	 * @param size Number of bytes to send.
	 * @return Status code.
	 **/
	Status Send(const void *data, std::size_t size);

	/**
	 * Send raw data to the remote peer. This function will fail if the socket is not connected.
	 * @param data Pointer to the sequence of bytes to send.
	 * @param size Number of bytes to send.
	 * @param sent The number of bytes sent will be written here.
	 * @return Status code.
	 **/
	Status Send(const void *data, std::size_t size, std::size_t &sent);

	/**
	 * Receive raw data from the remote peer.
	 * In blocking mode, this function will wait until some bytes are actually received.
	 * This function will fail if the socket is not connected.
	 * @param data Pointer to the array to fill with the received bytes.
	 * @param size Maximum number of bytes that can be received.
	 * @param received This variable is filled with the actual number of bytes received.
	 * @return Status code.
	 **/
	Status Receive(void *data, std::size_t size, std::size_t &received);

	/**
	 * Send a formatted packet of data to the remote peer.
	 * In non-blocking mode, if this function returns SOCKET_STATUS_PARTIAL, you \em must retry sending the same unmodified
	 * packet before sending anything else in order to guarantee the packet arrives at the remote peer uncorrupted.
	 * This function will fail if the socket is not connected.
	 * @param packet Packet to send.
	 * @return Status code.
	 **/
	Status Send(Packet &packet);

	/**
	 * Receive a formatted packet of data from the remote peer.
	 * In blocking mode, this function will wait until the whole packet has been received.
	 * This function will fail if the socket is not connected.
	 * @param packet Packet to fill with the received data.
	 * @return Status code.
	 **/
	Status Receive(Packet &packet);

private:
	/// Temporary data of the packet currently being received.
	PendingPacket m_pendingPacket;
};
}
