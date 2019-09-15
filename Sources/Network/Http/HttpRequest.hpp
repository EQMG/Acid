#pragma once

#include "StdAfx.hpp"

namespace acid {
/**
 * @brief Defines a HTTP request.
 **/
class ACID_EXPORT HttpRequest {
	friend class Http;
public:
	/**
	 * @brief Enumerate the available HTTP methods for a request, https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods.
	 **/
	enum class Method {
		/// Request in get mode, standard method to retrieve a page.
		Get,
		/// Request in post mode, usually to send data to a page.
		Post,
		/// Request a page's header only.
		Head,
		/// Request in put mode, useful for a REST API.
		Put,
		/// Request in delete mode, useful for a REST API.
		Delete,
		/// Request in options mode, describes the communication options for the page.
		Options,
		/// Request applies partial modifications to a page.
		Patch,
		/// Request performs a message loop-back test along the path to the target resource.
		Trace,
		/// Request establishes a tunnel to the server page.
		Connect
	};

	/**
	 * Default constructor, this constructor creates a GET request, with the root URI ("/") and an empty body.
	 * @param uri Target URI.
	 * @param method Method to use for the request.
	 * @param body Content of the request's body.
	 **/
	explicit HttpRequest(const std::string &uri = "/", const Method &method = Method::Get, const std::string &body = "");

	/**
	 * Set the value of a field.
	 * The field is created if it doesn't exist. The name of the field is case-insensitive.
	 * By default, a request doesn't contain any field (but the mandatory fields are added
	 * later by the HTTP client when sending the request).
	 * @param field Name of the field to set.
	 * @param value Value of the field.
	 **/
	void SetField(const std::string &field, const std::string &value);

	/**
	 * Set the request method.
	 * See the Method enumeration for a complete list of all the available methods.
	 * The method is HTTP_REQUEST_METHOD_GET by default.
	 * @param method Method to use for the request.
	 **/
	void SetMethod(const Method &method) { m_method = method; }

	/**
	 * Set the requested URI.
	 * The URI is the resource (usually a web page or a file) that you want to get or post.
	 * The URI is "/" (the root page) by default.
	 * @param uri URI to request, relative to the host.
	 **/
	void SetUri(const std::string &uri);

	/**
	 * Set the HTTP version for the request. The HTTP version is 1.0 by default.
	 * @param major Major HTTP version number.
	 * @param minor Minor HTTP version number.
	 **/
	void SetHttpVersion(uint32_t major, uint32_t minor);

	/**
	 * Set the body of the request.
	 * The body of a request is optional and only makes sense for POST requests.
	 * It is ignored for all other methods. The body is empty by default.
	 * @param body Content of the body.
	 **/
	void SetBody(const std::string &body) { m_body = body; }

private:
	using FieldTable = std::map<std::string, std::string>;

	/**
	 * Prepare the final request to send to the server.
	 * This is used internally by Http before sending the request to the web server.
	 * @return String containing the request, ready to be sent.
	 **/
	std::string Prepare() const;

	/**
	 * Check if the request defines a field. This function uses case-insensitive comparisons.
	 * @param field Name of the field to test.
	 * @return True if the field exists, false otherwise.
	 **/
	bool HasField(const std::string &field) const;

	/// Fields of the header associated to their value.
	FieldTable m_fields;
	/// Method to use for the request.
	Method m_method;
	/// Target URI of the request.
	std::string m_uri;
	/// Major HTTP version.
	uint32_t m_majorVersion;
	/// Minor HTTP version.
	uint32_t m_minorVersion;
	/// Body of the request.
	std::string m_body;
};
}
