#include "HttpRequest.hpp"

#include "Helpers/String.hpp"

namespace acid
{
HttpRequest::HttpRequest(const std::string &uri, const Method &method, const std::string &body) :
	m_method(method),
	m_majorVersion(1),
	m_minorVersion(0)
{
	SetUri(uri);
	SetBody(body);
}

void HttpRequest::SetField(const std::string &field, const std::string &value)
{
	m_fields[String::Lowercase(field)] = value;
}

void HttpRequest::SetUri(const std::string &uri)
{
	m_uri = uri;

	// Make sure it starts with a '/'.
	if (m_uri.empty() || (m_uri[0] != '/'))
	{
		m_uri.insert(0, "/");
	}
}

void HttpRequest::SetHttpVersion(const uint32_t &major, const uint32_t &minor)
{
	m_majorVersion = major;
	m_minorVersion = minor;
}

std::string HttpRequest::Prepare() const
{
	std::ostringstream out;

	// Convert the method to its string representation.
	std::string method;

	switch (m_method)
	{
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
	out << method << " " << m_uri << " ";
	out << "HTTP/" << m_majorVersion << "." << m_minorVersion << "\r\n";

	// Write fields.
	for (const auto &field : m_fields)
	{
		out << field.first << ": " << field.second << "\r\n";
	}

	// Use an extra \r\n to separate the header from the body.
	out << "\r\n";

	// Add the body.
	out << m_body;

	return out.str();
}

bool HttpRequest::HasField(const std::string &field) const
{
	return m_fields.find(String::Lowercase(field)) != m_fields.end();
}
}
