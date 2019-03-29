#pragma once

#include "Network/Socket.hpp"
#include "Network/IpAddress.hpp"

namespace acid
{
class Packet;

/// <summary>
/// A UDP socket is a connectionless socket. Instead of connecting once to a remote host,
/// like TCP sockets, it can send to and receive from any host at any time.
///
/// It is a datagram protocol: bounded blocks of data (datagrams) are transfered
/// over the network rather than a continuous stream of data (TCP).
/// Therefore, one call to send will always match one call to receive (if the datagram is not lost),
/// with the same data that was sent.
///
/// The UDP protocol is lightweight but unreliable. Unreliable means that datagrams may be duplicated,
/// be lost or arrive reordered. However, if a datagram arrives, its data is guaranteed to be valid.
///
/// UDP is generally used for real-time communication (audio or video streaming, real-time games, etc.)
/// where speed is crucial and lost data doesn't matter much.
///
/// Sending and receiving data can use either the low-level or the high-level functions.
/// The low-level functions process a raw sequence of bytes,
/// whereas the high-level interface uses packets (see acid::Packet),
/// which are easier to use and provide more safety regarding the data that is exchanged.
/// You can look at the acid::Packet class to get more details about how they work.
///
/// It is important to note that UdpSocket is unable to send datagrams bigger than MaxDatagramSize.
/// In this case, it returns an error and doesn't send anything. This applies to both raw data and packets.
/// Indeed, even packets are unable to split and recompose data, due to the unreliability of the protocol
/// (dropped, mixed or duplicated datagrams may lead to a big mess when trying to recompose a packet).
///
/// If the socket is bound to a port, it is automatically unbound from it when the socket is destroyed.
/// However, you can unbind the socket explicitly with the Unbind function if necessary,
/// to stop receiving messages or make the port available for other sockets.
/// </summary>
class ACID_EXPORT UdpSocket :
	public Socket
{
public:
	/// <summary>
	/// Default constructor.
	/// </summary>
	UdpSocket();

	/// <summary>
	/// Get the port to which the socket is bound locally. If the socket is not bound to a port, this function returns 0.
	/// </summary>
	/// <returns> Port to which the socket is bound. </returns>
	uint16_t GetLocalPort() const;

	/// <summary>
	/// Bind the socket to a specific port.
	/// Binding the socket to a port is necessary for being able to receive data on that port.
	/// You can use the special value Socket::AnyPort to tell the system to automatically pick an available port,
	/// and then call getLocalPort to retrieve the chosen port.
	/// Since the socket can only be bound to a single port at any given moment, if it is already bound when this function is called,
	/// it will be unbound from the previous port before being bound to the new one.
	/// </summary>
	/// <param name="port"> Port to bind the socket to. </param>
	/// <param name="address"> Address of the interface to bind to. </param>
	/// <returns> Status code. </returns>
	Status Bind(const uint16_t &port, const IpAddress &address = IpAddress::Any);

	/// <summary>
	/// Unbind the socket from the local port to which it is bound.
	/// The port that the socket was previously bound to is immediately made available to the operating system after this function is called.
	/// This means that a subsequent call to bind() will be able to re-bind the port if no other process has done so in the mean time.
	/// If the socket is not bound to a port, this function has no effect.
	/// </summary>
	void Unbind();

	/// <summary>
	/// Send raw data to a remote peer.
	/// Make sure that \a size is not greater than UdpSocket::MAX_DATAGRAM_SIZE, otherwise this function will fail and no data will be sent.
	/// </summary>
	/// <param name="data"> Pointer to the sequence of bytes to send. </param>
	/// <param name="size"> Number of bytes to send. </param>
	/// <param name="remoteAddress"> Address of the receiver. </param>
	/// <param name="remotePort"> Port of the receiver to send the data to. </param>
	/// <returns> Status code. </returns>
	Status Send(const void *data, const std::size_t &size, const IpAddress &remoteAddress, const uint16_t &remotePort);

	/// <summary>
	/// Receive raw data from a remote peer.
	/// In blocking mode, this function will wait until some bytes are actually received.
	/// Be careful to use a buffer which is large enough for the data that you intend to receive,
	/// if it is too small then an error will be returned and *all* the data will be lost.
	/// </summary>
	/// <param name="data"> Pointer to the array to fill with the received bytes. </param>
	/// <param name="size"> Maximum number of bytes that can be received. </param>
	/// <param name="received"> This variable is filled with the actual number of bytes received. </param>
	/// <param name="remoteAddress"> Address of the peer that sent the data. </param>
	/// <param name="remotePort"> Port of the peer that sent the data. </param>
	/// <returns> Status code. </returns>
	Status Receive(void *data, const std::size_t &size, std::size_t &received, IpAddress &remoteAddress, uint16_t &remotePort);

	/// <summary>
	/// Send a formatted packet of data to a remote peer.
	/// Make sure that the packet size is not greater than UdpSocket::MAX_DATAGRAM_SIZE, otherwise this function will fail and no data will be sent.
	/// </summary>
	/// <param name="packet"> Packet to send. </param>
	/// <param name="remoteAddress"> Address of the receiver. </param>
	/// <param name="remotePort"> Port of the receiver to send the data to. </param>
	/// <returns> Status code. </returns>
	Status Send(Packet &packet, const IpAddress &remoteAddress, const uint16_t &remotePort);

	/// <summary>
	/// Receive a formatted packet of data from a remote peer.
	/// In blocking mode, this function will wait until the whole packet has been received.
	/// </summary>
	/// <param name="packet"> Packet to fill with the received data. </param>
	/// <param name="remoteAddress"> Address of the peer that sent the data. </param>
	/// <param name="remotePort"> Port of the peer that sent the data. </param>
	/// <returns> Status code. </returns>
	Status Receive(Packet &packet, IpAddress &remoteAddress, uint16_t &remotePort);

private:
	/// Temporary buffer holding the received data in Receive(Packet).
	std::vector<char> m_buffer;
};
}
