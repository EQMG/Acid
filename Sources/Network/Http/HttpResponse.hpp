#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	///
	/// \brief Enumerate all the valid status codes for a response.
	///
	enum HttpResponseStatus
	{
		// 2xx: success.
		HTTP_RESPONCE_OK = 200, /// Most common code returned when operation was successful.
		HTTP_RESPONCE_CREATED = 201, /// The resource has successfully been created.
		HTTP_RESPONCE_ACCEPTED = 202, /// The request has been accepted, but will be processed later by the server.
		HTTP_RESPONCE_NO_CONTENT = 204, /// The server didn't send any data in return.
		HTTP_RESPONCE_RESET_CONTENT = 205, /// The server informs the client that it should clear the view (form) that caused the request to be sent.
		HTTP_RESPONCE_PARTIAL_CONTENT = 206, /// The server has sent a part of the resource, as a response to a partial GET request.

		// 3xx: redirection.
		HTTP_RESPONCE_MULTIPLE_CHOICES = 300, /// The requested page can be accessed from several locations.
		HTTP_RESPONCE_MOVED_PERMANENTLY = 301, /// The requested page has permanently moved to a new location.
		HTTP_RESPONCE_MOVED_TEMPORARILY = 302, /// The requested page has temporarily moved to a new location.
		HTTP_RESPONCE_NOT_MODIFIED = 304, /// For conditional requests, means the requested page hasn't changed and doesn't need to be refreshed.

		// 4xx: client error.
		HTTP_RESPONCE_BAD_REQUEST = 400, /// The server couldn't understand the request (syntax error).
		HTTP_RESPONCE_UNAUTHORIZED = 401, /// The requested page needs an authentication to be accessed.
		HTTP_RESPONCE_FORBIDDEN = 403, /// The requested page cannot be accessed at all, even with authentication.
		HTTP_RESPONCE_NOT_FOUND = 404, /// The requested page doesn't exist.
		HTTP_RESPONCE_Range_Not_Satisfiable = 407, /// The server can't satisfy the partial GET request (with a "Range" header field).

		// 5xx: server error.
		HTTP_RESPONCE_INTERNAL_SERVER_ERROR = 500, /// The server encountered an unexpected error.
		HTTP_RESPONCE_NOT_IMPLEMENTED = 501, /// The server doesn't implement a requested feature.
		HTTP_RESPONCE_BAD_GATEWAY = 502, /// The gateway server has received an error from the source server.
		HTTP_RESPONCE_SERVICE_NOT_AVAILABLE = 503, /// The server is temporarily unavailable (overloaded, in maintenance, ...).
		HTTP_RESPONCE_GATEWAY_TIMEOUT = 504, /// The gateway server couldn't receive a response from the source server.
		HTTP_RESPONCE_VERSION_NOT_SUPPORTED = 505, /// The server doesn't support the requested HTTP version.

		// 10xx: custom codes.
		HTTP_RESPONCE_INVALID_RESPONSE = 1000, /// Response is not a valid HTTP one.
		HTTP_RESPONCE_CONNECTION_FAILED = 1001  /// Connection with server failed.
	};

	///
	/// \brief Define a HTTP response.
	///
	class ACID_EXPORT HttpResponse
	{
	private:
		typedef std::map<std::string, std::string> FieldTable;

		FieldTable m_fields; /// Fields of the header.
		HttpResponseStatus m_status; /// Status code.
		unsigned int m_majorVersion; /// Major HTTP version.
		unsigned int m_minorVersion; /// Minor HTTP version.
		std::string m_body; /// Body of the response.
	public:
		///
		/// \brief Default constructor
		///
		/// Constructs an empty response.
		///
		HttpResponse();

		///
		/// \brief Get the value of a field
		///
		/// If the field \a field is not found in the response header,
		/// the empty string is returned. This function uses
		/// case-insensitive comparisons.
		///
		/// \param field Name of the field to get
		///
		/// \return Value of the field, or empty string if not found
		///
		const std::string &GetField(const std::string &field) const;

		///
		/// \brief Get the response status code
		///
		/// The status code should be the first thing to be checked
		/// after receiving a response, it defines whether it is a
		/// success, a failure or anything else (see the Status
		/// enumeration).
		///
		/// \return Status code of the response
		///
		HttpResponseStatus GetStatus() const;

		///
		/// \brief Get the major HTTP version number of the response
		///
		/// \return Major HTTP version number
		///
		/// \see getMinorHttpVersion
		///
		unsigned int GetMajorHttpVersion() const;

		///
		/// \brief Get the minor HTTP version number of the response
		///
		/// \return Minor HTTP version number
		///
		/// \see getMajorHttpVersion
		///
		unsigned int GetMinorHttpVersion() const;

		///
		/// \brief Get the body of the response
		///
		/// The body of a response may contain:
		/// \li the requested page (for GET requests)
		/// \li a response from the server (for POST requests)
		/// \li nothing (for HEAD requests)
		/// \li an error message (in case of an error)
		///
		/// \return The response body
		///
		const std::string &GetBody() const;
	private:
		friend class Http;

		///
		/// \brief Construct the header from a response string
		///
		/// This function is used by Http to build the response
		/// of a request.
		///
		/// \param data Content of the response to parse
		///
		void Parse(const std::string &data);

		///
		/// \brief Read values passed in the answer header
		///
		/// This function is used by Http to extract values passed
		/// in the response.
		///
		/// \param in String stream containing the header values
		///
		void ParseFields(std::istream &in);
	};
}
