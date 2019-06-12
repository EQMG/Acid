#include "Http.hpp"

#include "Engine/Log.hpp"
#include "Helpers/String.hpp"

namespace acid
{
Http::Http()
{
}

Http::Http(const std::string &host, const uint16_t &port) :
	m_port{port}
{
	SetHost(host, port);
}

void Http::SetHost(const std::string &host, const uint16_t &port)
{
	// Check the protocol.
	if (String::Lowercase(host.substr(0, 7)) == "http://")
	{
		// HTTP protocol.
		m_hostName = host.substr(7);
		m_port = port != 0 ? port : 80;
	}
	else if (String::Lowercase(host.substr(0, 8)) == "https://")
	{
		// HTTPS protocol -- unsupported (requires encryption and certificates and stuff...).
		Log::Error("HTTPS protocol is not supported by Http\n");
		m_hostName = "";
		m_port = 0;
	}
	else
	{
		// Undefined protocol - use HTTP.
		m_hostName = host;
		m_port = port != 0 ? port : 80;
	}

	// Remove any trailing '/' from the host name.
	if (!m_hostName.empty() && (*m_hostName.rbegin() == '/'))
	{
		m_hostName.erase(m_hostName.size() - 1);
	}

	m_host = {m_hostName};
}

HttpResponse Http::SendRequest(const HttpRequest &request, const Time &timeout)
{
	// First make sure that the request is valid -- add missing mandatory fields.
	HttpRequest toSend{request};

	if (!toSend.HasField("From"))
	{
		toSend.SetField("From", "user@sfml-dev.org");
	}

	if (!toSend.HasField("User-Agent"))
	{
		toSend.SetField("User-Agent", "libsfml-network/2.x");
	}

	if (!toSend.HasField("Host"))
	{
		toSend.SetField("Host", m_hostName);
	}

	if (!toSend.HasField("Content-Length"))
	{
		std::ostringstream out;
		out << toSend.m_body.size();
		toSend.SetField("Content-Length", out.str());
	}

	if ((toSend.m_method == HttpRequest::Method::Post) && !toSend.HasField("Content-Type"))
	{
		toSend.SetField("Content-Type", "application/x-www-form-urlencoded");
	}

	if ((toSend.m_majorVersion * 10 + toSend.m_minorVersion >= 11) && !toSend.HasField("Connection"))
	{
		toSend.SetField("Connection", "close");
	}

	// Prepare the response.
	HttpResponse received;

	// Connect the socket to the host.
	if (m_connection.Connect(m_host, m_port, timeout) == Socket::Status::Done)
	{
		// Convert the request to string and send it through the connected socket.
		auto requestStr{toSend.Prepare()};

		if (!requestStr.empty())
		{
			// Send it through the socket.
			if (m_connection.Send(requestStr.c_str(), requestStr.size()) == Socket::Status::Done)
			{
				// Wait for the server's response.
				std::string receivedStr;
				std::size_t size{};
				char buffer[1024];

				while (m_connection.Receive(buffer, sizeof(buffer), size) == Socket::Status::Done)
				{
					receivedStr.append(buffer, buffer + size);
				}

				// Build the Response object from the received data.
				received.Parse(receivedStr);
			}
		}

		// Close the connection.
		m_connection.Disconnect();
	}

	return received;
}
}
