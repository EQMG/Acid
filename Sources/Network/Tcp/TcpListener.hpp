#pragma once

#include "Network/Socket.hpp"
#include "Network/IpAddress.hpp"

namespace acid {
class TcpSocket;

/**
 * @brief A listener socket is a special type of socket that listens to a given port
 * and waits for connections on that port. This is all it can do.
 *
 * When a new connection is received, you must call accept and the listener returns a
 * new instance of acid::TcpSocket that is properly initialized and can be used to
 * communicate with the new client.
 *
 * Listener sockets are specific to the TCP protocol, UDP sockets are connectionless
 * and can therefore communicate directly. As a consequence,
 * a listener socket will always return the new connections as acid::TcpSocket instances.
 *
 * A listener is automatically closed on destruction, like all other types of socket.
 * However if you want to stop listening before the socket is destroyed, you can call its Close() function.
 **/
class ACID_EXPORT TcpListener : public Socket {
public:
	/**
	 * Default constructor.
	 **/
	TcpListener();

	/**
	 * Get the port to which the socket is bound locally.
	 * If the socket is not listening to a port, this function returns 0.
	 * @return Port to which the socket is bound.
	 **/
	uint16_t GetLocalPort() const;

	/**
	 * Start listening for incoming connection attempts.
	 * This function makes the socket start listening on the specified port, waiting for incoming connection attempts.
	 * If the socket is already listening on a port when this function is called,
	 * it will stop listening on the old port before starting to listen on the new port.
	 * @param port Port to listen on for incoming connection attempts.
	 * @param address Address of the interface to listen on.
	 * @return Status code.
	 **/
	Status Listen(uint16_t port, const IpAddress &address = IpAddress::Any);

	/**
	 * Stop listening and close the socket. This function gracefully stops the listener.
	 * If the socket is not listening, this function has no effect.
	 **/
	void Close();

	/**
	 * Accept a new connection.
	 * If the socket is in blocking mode, this function will not return until a connection is actually received.
	 * @param socket Socket that will hold the new connection.
	 * @return Status code.
	 **/
	Status Accept(TcpSocket &socket);
};
}
