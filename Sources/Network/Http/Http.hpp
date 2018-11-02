#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"
#include "Network/Tcp/TcpSocket.hpp"
#include "Network/IpAddress.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace acid
{
	///
	/// \brief A HTTP client
	///
	class ACID_EXPORT Http
	{
	private:
		TcpSocket m_connection; /// Connection to the host.
		IpAddress m_host; /// Web host address.
		std::string m_hostName; /// Web host name.
		unsigned short m_port; /// Port used for connection with host.
	public:
		///
		/// \brief Default constructor
		///
		Http();

		///
		/// \brief Construct the HTTP client with the target host
		///
		/// This is equivalent to calling setHost(host, port).
		/// The port has a default value of 0, which means that the
		/// HTTP client will use the right port according to the
		/// protocol used (80 for HTTP). You should leave it like
		/// this unless you really need a port other than the
		/// standard one, or use an unknown protocol.
		///
		/// \param host Web server to connect to
		/// \param port Port to use for connection
		///
		Http(const std::string &host, unsigned short port = 0);

		///
		/// \brief Set the target host
		///
		/// This function just stores the host address and port, it
		/// doesn't actually connect to it until you send a request.
		/// The port has a default value of 0, which means that the
		/// HTTP client will use the right port according to the
		/// protocol used (80 for HTTP). You should leave it like
		/// this unless you really need a port other than the
		/// standard one, or use an unknown protocol.
		///
		/// \param host Web server to connect to
		/// \param port Port to use for connection
		///
		void SetHost(const std::string &host, unsigned short port = 0);

		///
		/// \brief Send a HTTP request and return the server's response.
		///
		/// You must have a valid host before sending a request (see setHost).
		/// Any missing mandatory header field in the request will be added
		/// with an appropriate value.
		/// Warning: this function waits for the server's response and may
		/// not return instantly; use a thread if you don't want to block your
		/// application, or use a timeout to limit the time to wait. A value
		/// of Time::Zero means that the client will use the system default timeout
		/// (which is usually pretty long).
		///
		/// \param request Request to send
		/// \param timeout Maximum time to wait
		///
		/// \return Server's response
		///
		HttpResponse SendRequest(const HttpRequest &request, Time timeout = Time::ZERO);
	};
}
