#include "HttpRequest.hpp"

#include "Helpers/String.hpp"

namespace acid
{
	HttpRequest::HttpRequest(const std::string &uri, HttpRequestMethod method, const std::string &body)
	{
		SetMethod(method);
		SetUri(uri);
		SetHttpVersion(1, 0);
		SetBody(body);
	}

	void HttpRequest::SetField(const std::string &field, const std::string &value)
	{
		m_fields[String::Lowercase(field)] = value;
	}

	void HttpRequest::SetMethod(HttpRequestMethod method)
	{
		m_method = method;
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

	void HttpRequest::SetHttpVersion(uint32_t major, uint32_t minor)
	{
		m_majorVersion = major;
		m_minorVersion = minor;
	}

	void HttpRequest::SetBody(const std::string &body)
	{
		m_body = body;
	}

	std::string HttpRequest::Prepare() const
	{
		std::ostringstream out;

		// Convert the method to its string representation.
		std::string method;

		switch (m_method)
		{
			case HTTP_METHOD_GET:
				method = "GET";
				break;
			case HTTP_METHOD_POST:
				method = "POST";
				break;
			case HTTP_METHOD_HEAD:
				method = "HEAD";
				break;
			case HTTP_METHOD_PUT:
				method = "PUT";
				break;
			case HTTP_METHOD_DELETE:
				method = "DELETE";
				break;
		}

		// Write the first line containing the request type.
		out << method << " " << m_uri << " ";
		out << "HTTP/" << m_majorVersion << "." << m_minorVersion << "\r\n";

		// Write fields.
		for (FieldTable::const_iterator i = m_fields.begin(); i != m_fields.end(); ++i)
		{
			out << i->first << ": " << i->second << "\r\n";
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
