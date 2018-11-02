#pragma once

#include "Engine/Exports.hpp"
#include "Network/Tcp/TcpSocket.hpp"
#include "Network/IpAddress.hpp"
#include "FtpDataChannel.hpp"
#include "FtpResponse.hpp"
#include "FtpResponseDirectory.hpp"
#include "FtpResponseListing.hpp"

namespace acid
{
	/// <summary>
	/// A FTP client.
	/// </summary>
	class ACID_EXPORT Ftp
	{
	private:
		/// Socket holding the control connection with the server.
		TcpSocket m_commandSocket;
		/// Received command data that is yet to be processed.
		std::string m_receiveBuffer;
	public:
		/// <summary>
		/// Automatically closes the connection with the server if it is still opened.
		/// </summary>
		~Ftp();

		/// <summary>
		/// Connects to the specified FTP server.
		/// The port has a default value of 21, which is the standard port used by the FTP protocol.
		/// You shouldn't use a different value, unless you really know what you do.
		/// This function tries to connect to the server so it may take a while to complete,
		/// especially if the server is not reachable. To avoid blocking your application for too long,
		/// you can use a timeout. The default value, Time::Zero, means that the system timeout will be
		/// used (which is usually pretty long).
		/// </summary>
		/// <param name="server"> Name or address of the FTP server to connect to. </param>
		/// <param name="port"> Port used for the connection. </param>
		/// <param name="timeout"> Maximum time to wait. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse Connect(const IpAddress &server, unsigned short port = 21, Time timeout = Time::ZERO);

		/// <summary>
		/// Close the connection with the server.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponse Disconnect();

		/// <summary>
		/// Log in using an anonymous account.
		/// Logging in is mandatory after connecting to the server.
		/// Users that are not logged in cannot perform any operation.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponse Login();

		/// <summary>
		/// Log in using a username and a password.
		/// Logging in is mandatory after connecting to the server.
		/// Users that are not logged in cannot perform any operation.
		/// </summary>
		/// <param name="name"> User name. </param>
		/// <param name="password"> Password. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse Login(const std::string &name, const std::string &password);

		/// <summary>
		/// Send a null command to keep the connection alive.
		/// This command is useful because the server may close the connection automatically if no command is sent.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponse KeepAlive();

		/// <summary>
		/// Get the current working directory.
		/// The working directory is the root path for subsequent operations involving directories and/or filenames.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponseDirectory GetWorkingDirectory();

		/// <summary>
		/// Get the contents of the given directory.
		/// This function retrieves the sub-directories and files contained in the given directory. It is not recursive.
		/// The \a directory parameter is relative to the current working directory.
		/// </summary>
		/// <param name="directory"> Directory to list. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponseListing GetDirectoryListing(const std::string &directory = "");

		/// <summary>
		/// Change the current working directory.
		/// The new directory must be relative to the current one.
		/// </summary>
		/// <param name="directory"> New working directory. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse ChangeDirectory(const std::string &directory);

		/// <summary>
		/// Go to the parent directory of the current one.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponse ParentDirectory();

		/// <summary>
		/// Create a new directory.
		/// The new directory is created as a child of the current working directory.
		/// </summary>
		/// <param name="name"> Name of the directory to create. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse CreateRemoteDirectory(const std::string &name);

		/// <summary>
		/// Remove an existing directory
		/// The directory to remove must be relative to the current working directory.
		/// Use this function with caution, the directory will be removed permanently!
		/// </summary>
		/// <param name="name"> Name of the directory to remove. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse DeleteRemoteDirectory(const std::string &name);

		/// <summary>
		/// Rename an existing file.
		/// The filenames must be relative to the current working directory.
		/// </summary>
		/// <param name="file"> File to rename. </param>
		/// <param name="newName"> New name of the file. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse RenameRemoteFile(const std::string &file, const std::string &newName);

		/// <summary>
		/// Remove an existing file.
		/// The file name must be relative to the current working directory.
		/// Use this function with caution, the file will be removed permanently!
		/// </summary>
		/// <param name="name"> File to remove. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse DeleteRemoteFile(const std::string &name);

		/// <summary>
		/// Download a file from the server.
		/// The filename of the distant file is relative to the current working directory of the server,
		/// and the local destination path is relative to the current directory of your application.
		/// If a file with the same filename as the distant file already exists in the local destination path,
		/// it will be overwritten.
		/// </summary>
		/// <param name="remoteFile"> Filename of the distant file to download. </param>
		/// <param name="localPath"> The directory in which to put the file on the local computer. </param>
		/// <param name="mode"> Transfer mode. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse Download(const std::string &remoteFile, const std::string &localPath, FtpTransferMode mode = FTP_MODE_BINARY);

		/// <summary>
		/// Upload a file to the server.
		/// The name of the local file is relative to the current working directory of your application,
		/// and the remote path is relative to the current directory of the FTP server.
		/// The append parameter controls whether the remote file is appended to or overwritten if it already exists.
		/// </summary>
		/// <param name="localFile"> Path of the local file to upload. </param>
		/// <param name="remotePath"> The directory in which to put the file on the server. </param>
		/// <param name="mode"> Transfer mode. </param>
		/// <param name="append"> Pass true to append to or false to overwrite the remote file if it already exists. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse Upload(const std::string &localFile, const std::string &remotePath, FtpTransferMode mode = FTP_MODE_BINARY, bool append = false);

		/// <summary>
		/// Send a command to the FTP server.
		/// While the most often used commands are provided as member functions in the Ftp class,
		/// this method can be used to send any FTP command to the server.
		/// If the command requires one or more parameters, they can be specified in \a parameter.
		/// If the server returns information, you can extract it from the response using FtpResponse::GetFullMessage().
		/// </summary>
		/// <param name="command"> Command to send. </param>
		/// <param name="parameter"> Command parameter. </param>
		/// <returns> Server response to the request. </returns>
		FtpResponse SendCommand(const std::string &command, const std::string &parameter = "");
	private:
		/// <summary>
		/// Receive a response from the server.
		/// This function must be called after each call to SendCommand that expects a response.
		/// </summary>
		/// <returns> Server response to the request. </returns>
		FtpResponse GetResponse();
	};
}
