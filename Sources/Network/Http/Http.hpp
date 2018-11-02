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
	/// <summary>
	/// A HTTP client.
	/// </summary>
	class ACID_EXPORT Http
	{
	private:
		/// Connection to the host.
		TcpSocket m_connection;
		/// Web host address.
		IpAddress m_host;
		/// Web host name.
		std::string m_hostName;
		/// Port used for connection with host.
		unsigned short m_port;
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		Http();

		/// <summary>
		/// Construct the HTTP client with the target host.
		/// This is equivalent to calling setHost(host, port).
		/// The port has a default value of 0, which means that the HTTP client will use the
		/// right port according to the protocol used (80 for HTTP).
		/// You should leave it like this unless you really need a port other than the standard one, or use an unknown protocol.
		/// </summary>
		/// <param name="host"> Web server to connect to. </param>
		/// <param name="port"> Port to use for connection. </param>
		Http(const std::string &host, unsigned short port = 0);

		/// <summary>
		/// Set the target host.
		/// This function just stores the host address and port, it doesn't actually connect to it until you send a request.
		/// The port has a default value of 0, which means that the HTTP client will use the right port according to the
		/// protocol used (80 for HTTP). You should leave it like this unless you really need a port other than the
		/// standard one, or use an unknown protocol.
		/// </summary>
		/// <param name="host"> Web server to connect to. </param>
		/// <param name="port"> Port to use for connection. </param>
		void SetHost(const std::string &host, unsigned short port = 0);

		/// <summary>
		/// Send a HTTP request and return the server's response.
		/// You must have a valid host before sending a request (see setHost).
		/// Any missing mandatory header field in the request will be added with an appropriate value.
		/// Warning: this function waits for the server's response and may not return instantly;
		/// use a thread if you don't want to block your application, or use a timeout to limit the time to wait.
		/// A value of Time::ZERO means that the client will use the system default timeout (which is usually pretty long).
		/// </summary>
		/// <param name="request"> Request to send. </param>
		/// <param name="timeout"> Maximum time to wait. </param>
		/// <returns> Server's response. </returns>
		HttpResponse SendRequest(const HttpRequest &request, Time timeout = Time::ZERO);
	};
}
