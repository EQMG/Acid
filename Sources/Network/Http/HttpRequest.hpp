#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Enumerate the available HTTP methods for a request, https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods.
	/// </summary>
	enum HttpRequestMethod
	{
		/// Request in get mode, standard method to retrieve a page.
		HTTP_METHOD_GET = 0,
		/// Request in post mode, usually to send data to a page.
		HTTP_METHOD_POST = 1,
		/// Request a page's header only.
		HTTP_METHOD_HEAD = 2,
		/// Request in put mode, useful for a REST API.
		HTTP_METHOD_PUT = 3,
		/// Request in delete mode, useful for a REST API.
		HTTP_METHOD_DELETE = 4,
		/// Request in options mode, describes the communication options for the page.
		HTTP_METHOD_OPTIONS = 5,
		/// Request applies partial modifications to a page.
		HTTP_METHOD_PATCH = 6,
		/// Request performs a message loop-back test along the path to the target resource.
		HTTP_METHOD_TRACE = 7,
		/// Request establishes a tunnel to the server page.
		HTTP_METHOD_CONNECT = 8
	};

	/// <summary>
	/// Define a HTTP request.
	/// </summary>
	class ACID_EXPORT HttpRequest
	{
	private:
		typedef std::map<std::string, std::string> FieldTable;

		/// Fields of the header associated to their value.
		FieldTable m_fields;
		/// Method to use for the request.
		HttpRequestMethod m_method;
		/// Target URI of the request.
		std::string m_uri;
		/// Major HTTP version.
		uint32_t m_majorVersion;
		/// Minor HTTP version.
		uint32_t m_minorVersion;
		/// Body of the request.
		std::string m_body;
	public:
		/// <summary>
		/// Default constructor, this constructor creates a GET request, with the root URI ("/") and an empty body.
		/// </summary>
		/// <param name="uri"> Target URI. </param>
		/// <param name="method"> Method to use for the request. </param>
		/// <param name="body"> Content of the request's body. </param>
		HttpRequest(const std::string &uri = "/", HttpRequestMethod method = HTTP_METHOD_GET, const std::string &body = "");

		/// <summary>
		/// Set the value of a field.
		/// The field is created if it doesn't exist. The name of the field is case-insensitive.
		/// By default, a request doesn't contain any field (but the mandatory fields are added
		/// later by the HTTP client when sending the request).
		/// </summary>
		/// <param name="field"> Name of the field to set. </param>
		/// <param name="value"> Value of the field. </param>
		void SetField(const std::string &field, const std::string &value);

		/// <summary>
		/// Set the request method.
		/// See the Method enumeration for a complete list of all the available methods.
		/// The method is HTTP_REQUEST_METHOD_GET by default.
		/// </summary>
		/// <param name="method"> Method to use for the request. </param>
		void SetMethod(HttpRequestMethod method);

		/// <summary>
		/// Set the requested URI.
		/// The URI is the resource (usually a web page or a file) that you want to get or post.
		/// The URI is "/" (the root page) by default.
		/// </summary>
		/// <param name="uri"> URI to request, relative to the host. </param>
		void SetUri(const std::string &uri);

		/// <summary>
		/// Set the HTTP version for the request. The HTTP version is 1.0 by default.
		/// </summary>
		/// <param name="major"> Major HTTP version number. </param>
		/// <param name="minor"> Minor HTTP version number. </param>
		void SetHttpVersion(uint32_t major, uint32_t minor);

		/// <summary>
		/// Set the body of the request.
		/// The body of a request is optional and only makes sense for POST requests.
		/// It is ignored for all other methods. The body is empty by default.
		/// </summary>
		/// <param name="body"> Content of the body. </param>
		void SetBody(const std::string &body);
	private:
		friend class Http;

		/// <summary>
		/// Prepare the final request to send to the server.
		/// This is used internally by Http before sending the request to the web server.
		/// </summary>
		/// <returns> String containing the request, ready to be sent. </returns>
		std::string Prepare() const;

		/// <summary>
		/// Check if the request defines a field. This function uses case-insensitive comparisons.
		/// </summary>
		/// <param name="field"> Name of the field to test. </param>
		/// <returns> True if the field exists, false otherwise. </returns>
		bool HasField(const std::string &field) const;
	};
}
