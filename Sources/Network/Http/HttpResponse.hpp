#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Enumerate all the valid status codes for a response.
	/// </summary>
	enum HttpResponseStatus
	{
		// 2xx: success.
		/// Most common code returned when operation was successful.
		HTTP_RESPONSE_OK = 200,
		/// The resource has successfully been created.
		HTTP_RESPONSE_CREATED = 201,
		/// The request has been accepted, but will be processed later by the server.
		HTTP_RESPONSE_ACCEPTED = 202,
		/// The server didn't send any data in return.
		HTTP_RESPONSE_NO_CONTENT = 204,
		/// The server informs the client that it should clear the view (form) that caused the request to be sent.
		HTTP_RESPONSE_RESET_CONTENT = 205,
		/// The server has sent a part of the resource, as a response to a partial GET request.
		HTTP_RESPONSE_PARTIAL_CONTENT = 206,

		// 3xx: redirection.
		/// The requested page can be accessed from several locations.
		HTTP_RESPONSE_MULTIPLE_CHOICES = 300,
		/// The requested page has permanently moved to a new location.
		HTTP_RESPONSE_MOVED_PERMANENTLY = 301,
		/// The requested page has temporarily moved to a new location.
		HTTP_RESPONSE_MOVED_TEMPORARILY = 302,
		/// For conditional requests, means the requested page hasn't changed and doesn't need to be refreshed.
		HTTP_RESPONSE_NOT_MODIFIED = 304,

		// 4xx: client error.
		/// The server couldn't understand the request (syntax error).
		HTTP_RESPONSE_BAD_REQUEST = 400,
		/// The requested page needs an authentication to be accessed.
		HTTP_RESPONSE_UNAUTHORIZED = 401,
		/// The requested page cannot be accessed at all, even with authentication.
		HTTP_RESPONSE_FORBIDDEN = 403,
		/// The requested page doesn't exist.
		HTTP_RESPONSE_NOT_FOUND = 404,
		/// The server can't satisfy the partial GET request (with a "Range" header field).
		HTTP_RESPONSE_RANGE_NOT_SATISFIABLE = 407,

		// 5xx: server error.
		/// The server encountered an unexpected error.
		HTTP_RESPONSE_INTERNAL_SERVER_ERROR = 500,
		/// The server doesn't implement a requested feature.
		HTTP_RESPONSE_NOT_IMPLEMENTED = 501,
		/// The gateway server has received an error from the source server.
		HTTP_RESPONSE_BAD_GATEWAY = 502,
		/// The server is temporarily unavailable (overloaded, in maintenance, ...).
		HTTP_RESPONSE_SERVICE_NOT_AVAILABLE = 503,
		/// The gateway server couldn't receive a response from the source server.
		HTTP_RESPONSE_GATEWAY_TIMEOUT = 504,
		/// The server doesn't support the requested HTTP version.
		HTTP_RESPONSE_VERSION_NOT_SUPPORTED = 505,

		// 10xx: custom codes.
		/// Response is not a valid HTTP one.
		HTTP_RESPONSE_INVALID_RESPONSE = 1000,
		/// Connection with server failed.
		HTTP_RESPONSE_CONNECTION_FAILED = 1001
	};

	/// <summary>
	/// Define a HTTP response.
	/// </summary>
	class ACID_EXPORT HttpResponse
	{
	private:
		typedef std::map<std::string, std::string> FieldTable;

		/// Fields of the header.
		FieldTable m_fields;
		/// Status code.
		HttpResponseStatus m_status;
		/// Major HTTP version.
		uint32_t m_majorVersion;
		/// Minor HTTP version.
		uint32_t m_minorVersion;
		/// Body of the response.
		std::string m_body;
	public:
		/// <summary>
		/// Default constructor, constructs an empty response.
		/// </summary>
		HttpResponse();

		/// <summary>
		/// Get the value of a field.
		/// If the field \a field is not found in the response header, the empty string is returned.
		/// This function uses case-insensitive comparisons.
		/// </summary>
		/// <param name="field"> Name of the field to get. </param>
		/// <returns> Value of the field, or empty string if not found. </returns>
		const std::string &GetField(const std::string &field) const;

		/// <summary>
		/// Get the response status code.
		/// The status code should be the first thing to be checked after receiving a response,
		/// it defines whether it is a success, a failure or anything else (see the Status enumeration).
		/// </summary>
		/// <returns> Status code of the response. </returns>
		HttpResponseStatus GetStatus() const;

		/// <summary>
		/// Get the major HTTP version number of the response.
		/// </summary>
		/// <returns> Major HTTP version number. </returns>
		uint32_t GetMajorHttpVersion() const;

		/// <summary>
		/// Get the minor HTTP version number of the response.
		/// </summary>
		/// <returns> Minor HTTP version number. </returns>
		uint32_t GetMinorHttpVersion() const;

		/// <summary>
		/// Get the body of the response.
		/// The body of a response may contain:
		/// <ul>
		/// <li>The requested page (for GET requests).</li>
		/// <li>A response from the server (for POST requests).</li>
		/// <li>Nothing (for HEAD requests).</li>
		/// <li>An error message (in case of an error).</li>
		/// </ul>
		/// </summary>
		/// <returns> The response body. </returns>
		const std::string &GetBody() const;
	private:
		friend class Http;

		/// <summary>
		/// Construct the header from a response string.
		/// This function is used by Http to build the response of a request.
		/// </summary>
		/// <param name="data"> Content of the response to parse. </param>
		void Parse(const std::string &data);

		/// <summary>
		/// Read values passed in the answer header.
		/// This function is used by Http to extract values passed in the response.
		/// </summary>
		/// <param name="in"> String stream containing the header values. </param>
		void ParseFields(std::istream &in);
	};
}
