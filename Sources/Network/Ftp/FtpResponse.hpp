#pragma once

#include <string>
#include "Engine/Exports.hpp"

namespace acid
{
	/// <summary>
	/// Status codes possibly returned by a FTP response.
	/// </summary>
	enum FtpResponseStatus
	{
		// 1xx: the requested action is being initiated, expect another reply before proceeding with a new command.
		/// Restart marker reply.
		FTP_RESPONSE_RESTART_MARKER_REPLY = 110,
		/// Service ready in N minutes.
		FTP_RESPONSE_SERVICE_READY_SOON = 120,
		/// Data connection already opened, transfer starting.
		FTP_RESPONSE_DATA_CONNECTION_ALREADY_OPENED = 125,
		/// File status ok, about to open data connection.
		FTP_RESPONSE_OPENING_DATA_CONNECTION = 150,

		// 2xx: the requested action has been successfully completed.
		/// Command ok.
		FTP_RESPONSE_OK = 200,
		/// Command not implemented.
		FTP_RESPONSE_POINTLESS_COMMAND = 202,
		/// System status, or system help reply.
		FTP_RESPONSE_SYSTEM_STATUS = 211,
		/// Directory status.
		FTP_RESPONSE_DIRECTORY_STATUS = 212,
		/// File status.
		FTP_RESPONSE_FILE_STATUS = 213,
		/// Help message.
		FTP_RESPONSE_HELP_MESSAGE = 214,
		/// NAME system type, where NAME is an official system name from the list in the Assigned Numbers document.
		FTP_RESPONSE_SYSTEM_TYPE = 215,
		/// Service ready for new user.
		FTP_RESPONSE_SERVICE_READY = 220,
		/// Service closing control connection.
		FTP_RESPONSE_CLOSING_CONNECTION = 221,
		/// Data connection open, no transfer in progress.
		FTP_RESPONSE_DATA_CONNECTION_OPENED = 225,
		/// Closing data connection, requested file action successful.
		FTP_RESPONSE_CLOSING_DATA_CONNECTION = 226,
		/// Entering passive mode.
		FTP_RESPONSE_ENTERING_PASSIVE_MODE = 227,
		/// User logged in, proceed. Logged out if appropriate.
		FTP_RESPONSE_LOGGEDIN = 230,
		/// Requested file action ok.
		FTP_RESPONSE_FILEACTION_OK = 250,
		/// PATHNAME created.
		FTP_RESPONSE_DIRECTORY_OK = 257,

		// 3xx: the command has been accepted, but the requested action is dormant, pending receipt of further information.
		/// User name ok, need password.
		FTP_RESPONSE_NEED_PASSWORD = 331,
		/// Need account for login.
		FTP_RESPONSE_NEED_ACCOUNT_TO_LOGIN = 332,
		/// Requested file action pending further information.
		FTP_RESPONSE_NEED_INFORMATION = 350,

		// 4xx: the command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again.
		/// Service not available, closing control connection.
		FTP_RESPONSE_SERVICE_UNAVAILABLE = 421,
		/// Can't open data connection.
		FTP_RESPONSE_DATA_CONNECTION_UNAVAILABLE = 425,
		/// Connection closed, transfer aborted.
		FTP_RESPONSE_TRANSFER_ABORTED = 426,
		/// Requested file action not taken.
		FTP_RESPONSE_FILE_ACTION_ABORTED = 450,
		/// Requested action aborted, local error in processing.
		FTP_RESPONSE_LOCAL_ERROR = 451,
		/// Requested action not taken; insufficient storage space in system, file unavailable.
		FTP_RESPONSE_INSUFFICIENT_STORAGE_SPACE = 452,

		// 5xx: the command was not accepted and the requested action did not take place.
		/// Syntax error, command unrecognized.
		FTP_RESPONSE_COMMAND_UNKNOWN = 500,
		/// Syntax error in parameters or arguments.
		FTP_RESPONSE_PARAMETERS_UNKNOWN = 501,
		/// Command not implemented.
		FTP_RESPONSE_COMMAND_NOT_IMPLEMENTED = 502,
		/// Bad sequence of commands.
		FTP_RESPONSE_BAD_COMMAND_SEQUENCE = 503,
		/// Command not implemented for that parameter.
		FTP_RESPONSE_PARAMETER_NOT_IMPLEMENTED = 504,
		/// Not logged in.
		FTP_RESPONSE_NOT_LOGGEDIN = 530,
		/// Need account for storing files.
		FTP_RESPONSE_NEED_ACCOUNT_TO_STORE = 532,
		/// Requested action not taken, file unavailable.
		FTP_RESPONSE_FILE_UNAVAILABLE = 550,
		/// Requested action aborted, page type unknown.
		FTP_RESPONSE_PAGE_TYPE_UNKNOWN = 551,
		/// Requested file action aborted, exceeded storage allocation.
		FTP_RESPONSE_NOT_ENOUGH_MEMORY = 552,
		/// Requested action not taken, file name not allowed.
		FTP_RESPONSE_FILENAME_NOT_ALLOWED = 553,

		// 10xx: custom codes.
		/// Not part of the FTP standard, generated when a received response cannot be parsed.
		FTP_RESPONSE_INVALID_RESPONSE = 1000,
		/// Not part of the FTP standard, generated when the low-level socket connection with the server fails.
		FTP_RESPONSE_CONNECTION_FAILED = 1001,
		/// Not part of the FTP standard, generated when the low-level socket connection is unexpectedly closed.
		FTP_RESPONSE_CONNECTION_CLOSED = 1002,
		/// Not part of the FTP standard, generated when a local file cannot be read or written.
		FTP_RESPONSE_INVALID_FILE = 1003
	};

	/// <summary>
	/// Define a FTP response.
	/// </summary>
	class ACID_EXPORT FtpResponse
	{
	private:
		/// Status code returned from the server.
		FtpResponseStatus m_status;
		/// Last message received from the server.
		std::string m_message;
	public:
		/// <summary>
		/// Default constructor, this constructor is used by the FTP client to build the response.
		/// </summary>
		/// <param name="code"> Response status code. </param>
		/// <param name="message"> Response message. </param>
		explicit FtpResponse(FtpResponseStatus code = FTP_RESPONSE_INVALID_RESPONSE, const std::string &message = "");

		/// <summary>
		/// Check if the status code means a success.
		/// This function is defined for convenience, it is equivalent to testing if the status code is < 400.
		/// </summary>
		/// <returns> True if the status is a success, false if it is a failure. </returns>
		bool IsOk() const;

		/// <summary>
		/// Get the status code of the response.
		/// </summary>
		/// <returns> Status code. </returns>
		FtpResponseStatus GetStatus() const;

		/// <summary>
		/// Get the full message contained in the response.
		/// </summary>
		/// <returns> The response message. </returns>
		const std::string &GetFullMessage() const;
	};
}
