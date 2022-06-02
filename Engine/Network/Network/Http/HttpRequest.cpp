#include "HttpRequest.hpp"

#include "Utils/String.hpp"

namespace acid {
HttpRequest::HttpRequest(const std::string &uri, Method method, const std::string &body) :
	method(method),
	majorVersion(1),
	minorVersion(0) {
	SetUri(uri);
	SetBody(body);
}

void HttpRequest::SetField(const std::string &field, const std::string &value) {
	fields[String::Lowercase(field)] = value;
}

void HttpRequest::SetUri(const std::string &uri) {
	this->uri = uri;

	// Make sure it starts with a '/'.
	if (this->uri.empty() || this->uri[0] != '/')
		this->uri.insert(0, "/");
}

void HttpRequest::SetHttpVersion(uint32_t major, uint32_t minor) {
	majorVersion = major;
	minorVersion = minor;
}

std::string HttpRequest::Prepare() const {
	std::ostringstream out;

	// Convert the method to its string representation.
	std::string method;

	switch (this->method) {
	case Method::Get:
		method = "GET";
		break;
	case Method::Post:
		method = "POST";
		break;
	case Method::Head:
		method = "HEAD";
		break;
	case Method::Put:
		method = "PUT";
		break;
	case Method::Delete:
		method = "DELETE";
		break;
	case Method::Options:
		method = "OPTIONS";
		break;
	case Method::Patch:
		method = "PATCH";
		break;
	case Method::Trace:
		method = "TRACE";
		break;
	case Method::Connect:
		method = "CONNECT";
		break;
	}

	// Write the first line containing the request type.
	out << method << " " << uri << " ";
	out << "HTTP/" << majorVersion << "." << minorVersion << "\r\n";

	// Write fields.
	for (const auto &[fieldName, fieldValue] : fields)
		out << fieldName << ": " << fieldValue << "\r\n";

	// Use an extra \r\n to separate the header from the body.
	out << "\r\n";

	// Add the body.
	out << body;

	return out.str();
}

bool HttpRequest::HasField(const std::string &field) const {
	return fields.find(String::Lowercase(field)) != fields.end();
}
}
