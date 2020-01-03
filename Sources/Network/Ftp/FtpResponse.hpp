#pragma once

#include <string>

#include "Export.hpp"

namespace acid {
/**
 * @brief Define a FTP response.
 **/
class ACID_EXPORT FtpResponse {
public:
	/**
	 * @brief Status codes possibly returned by a FTP response.
	 **/
	enum class Status {
		// 1xx: the requested action is being initiated, expect another reply before proceeding with a new command.

		/// Restart marker reply.
		RestartMarkerReply = 110,
		/// Service ready in N minutes.
		ServiceReadySoon = 120,
		/// Data connection already opened, transfer starting.
		DataConnectionAlreadyOpened = 125,
		/// File status ok, about to open data connection.
		OpeningDataConnection = 150,

		// 2xx: the requested action has been successfully completed.

		/// Command ok.
		Ok = 200,
		/// Command not implemented.
		PointlessCommand = 202,
		/// System status, or system help reply.
		SystemStatus = 211,
		/// Directory status.
		DirectoryStatus = 212,
		/// File status.
		FileStatus = 213,
		/// Help message.
		HelpMessage = 214,
		/// NAME system type, where NAME is an official system name from the list in the Assigned Numbers document.
		SystemType = 215,
		/// Service ready for new user.
		ServiceReady = 220,
		/// Service closing control connection.
		ClosingConnection = 221,
		/// Data connection open, no transfer in progress.
		DataConnectionOpened = 225,
		/// Closing data connection, requested file action successful.
		ClosingDataConnection = 226,
		/// Entering passive mode.
		EnteringPassiveMode = 227,
		/// User logged in, proceed. Logged out if appropriate.
		LoggedIn = 230,
		/// Requested file action ok.
		FileActionOk = 250,
		/// PATHNAME created.
		DirectoryOk = 257,

		// 3xx: the command has been accepted, but the requested action is dormant, pending receipt of further information.

		/// User name ok, need password.
		NeedPassword = 331,
		/// Need account for login.
		NeedAccountToLogIn = 332,
		/// Requested file action pending further information.
		NeedInformation = 350,

		// 4xx: the command was not accepted and the requested action did not take place, but the error condition is temporary and the action may be requested again.

		/// Service not available, closing control connection.
		ServiceUnavailable = 421,
		/// Can't open data connection.
		DataConnectionUnavailable = 425,
		/// Connection closed, transfer aborted.
		TransferAborted = 426,
		/// Requested file action not taken.
		FileActionAborted = 450,
		/// Requested action aborted, local error in processing.
		LocalError = 451,
		/// Requested action not taken; insufficient storage space in system, file unavailable.
		InsufficientStorageSpace = 452,

		// 5xx: the command was not accepted and the requested action did not take place.

		/// Syntax error, command unrecognized.
		CommandUnknown = 500,
		/// Syntax error in parameters or arguments.
		ParametersUnknown = 501,
		/// Command not implemented.
		CommandNotImplemented = 502,
		/// Bad sequence of commands.
		BadCommandSequence = 503,
		/// Command not implemented for that parameter.
		ParameterNotImplemented = 504,
		/// Not logged in.
		NotLoggedIn = 530,
		/// Need account for storing files.
		NeedAccountToStore = 532,
		/// Requested action not taken, file unavailable.
		FileUnavailable = 550,
		/// Requested action aborted, page type unknown.
		PageTypeUnknown = 551,
		/// Requested file action aborted, exceeded storage allocation.
		NotEnoughMemory = 552,
		/// Requested action not taken, file name not allowed.
		FilenameNotAllowed = 553,

		// 10xx: custom codes.
		/// Not part of the FTP standard, generated when a received response cannot be parsed.
		InvalidResponse = 1000,
		/// Not part of the FTP standard, generated when the low-level socket connection with the server fails.
		ConnectionFailed = 1001,
		/// Not part of the FTP standard, generated when the low-level socket connection is unexpectedly closed.
		ConnectionClosed = 1002,
		/// Not part of the FTP standard, generated when a local file cannot be read or written.
		InvalidFile = 1003
	};

	/**
	 * Default constructor, this constructor is used by the FTP client to build the response.
	 * @param code Response status code.
	 * @param message Response message.
	 **/
	FtpResponse(Status code = Status::InvalidResponse, std::string message = "");

	/**
	 * Check if the status code means a success.
	 * This function is defined for convenience, it is equivalent to testing if the status code is < 400.
	 * @return True if the status is a success, false if it is a failure.
	 **/
	bool IsOk() const;

	/**
	 * Get the status code of the response.
	 * @return Status code.
	 **/
	Status GetStatus() const { return m_status; }

	/**
	 * Get the full message contained in the response.
	 * @return The response message.
	 **/
	const std::string &GetFullMessage() const { return m_message; }

private:
	/// Status code returned from the server.
	Status m_status;
	/// Last message received from the server.
	std::string m_message;
};
}
