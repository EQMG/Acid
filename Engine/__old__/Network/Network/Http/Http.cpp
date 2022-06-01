#include "Http.hpp"

#include "Engine/Log.hpp"
#include "Utils/String.hpp"

namespace acid {
Http::Http() {
}

Http::Http(const std::string &host, uint16_t port) :
	port(port) {
	SetHost(host, port);
}

void Http::SetHost(const std::string &host, uint16_t port) {
	// Check the protocol.
	if (String::Lowercase(host.substr(0, 7)) == "http://") {
		// HTTP protocol.
		hostName = host.substr(7);
		this->port = port != 0 ? port : 80;
	} else if (String::Lowercase(host.substr(0, 8)) == "https://") {
		// HTTPS protocol -- unsupported (requires encryption and certificates and stuff...).
		Log::Error("HTTPS protocol is not supported by HTTP\n");
		hostName = "";
		this->port = 0;
	} else {
		// Undefined protocol - use HTTP.
		hostName = host;
		this->port = port != 0 ? port : 80;
	}

	// Remove any trailing '/' from the host name.
	if (!hostName.empty() && (*hostName.rbegin() == '/'))
		hostName.erase(hostName.size() - 1);

	this->host = {hostName};
}

HttpResponse Http::SendRequest(const HttpRequest &request, const Time &timeout) {
	// First make sure that the request is valid -- add missing mandatory fields.
	HttpRequest toSend(request);

	if (!toSend.HasField("From"))
		toSend.SetField("From", "user@sfml-dev.org");

	if (!toSend.HasField("User-Agent"))
		toSend.SetField("User-Agent", "libsfml-network/2.x");

	if (!toSend.HasField("Host"))
		toSend.SetField("Host", hostName);

	if (!toSend.HasField("Content-Length")) {
		std::ostringstream out;
		out << toSend.body.size();
		toSend.SetField("Content-Length", out.str());
	}

	if (toSend.method == HttpRequest::Method::Post && !toSend.HasField("Content-Type"))
		toSend.SetField("Content-Type", "application/x-www-form-urlencoded");

	if (toSend.majorVersion * 10 + toSend.minorVersion >= 11 && !toSend.HasField("Connection"))
		toSend.SetField("Connection", "close");

	// Prepare the response.
	HttpResponse received;

	// Connect the socket to the host.
	if (connection.Connect(host, port, timeout) == Socket::Status::Done) {
		// Convert the request to string and send it through the connected socket.
		auto requestStr = toSend.Prepare();

		if (!requestStr.empty()) {
			// Send it through the socket.
			if (connection.Send(requestStr.c_str(), requestStr.size()) == Socket::Status::Done) {
				// Wait for the server's response.
				std::string receivedStr;
				std::size_t size = 0;
				char buffer[1024];

				while (connection.Receive(buffer, sizeof(buffer), size) == Socket::Status::Done)
					receivedStr.append(buffer, buffer + size);

				// Build the Response object from the received data.
				received.Parse(receivedStr);
			}
		}

		// Close the connection.
		connection.Disconnect();
	}

	return received;
}
}
