#include "Ftp.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <fstream>
#include <iterator>
#include <cstdio>
#include "Network/IpAddress.hpp"

namespace acid
{
	Ftp::Ftp() :
		m_commandSocket(TcpSocket()),
		m_receiveBuffer("")
	{
	}

	Ftp::~Ftp()
	{
		Disconnect();
	}

	FtpResponse Ftp::Connect(const IpAddress &server, const uint16_t &port, const Time &timeout)
	{
		// Connect to the server.
		if (m_commandSocket.Connect(server, port, timeout) != Socket::Status::Done)
		{
			return FtpResponse(FtpResponse::Status::ConnectionFailed);
		}

		// Get the response to the connection.
		return GetResponse();
	}

	FtpResponse Ftp::Login()
	{
		return Login("anonymous", "user@sfml-dev.org");
	}

	FtpResponse Ftp::Login(const std::string &name, const std::string &password)
	{
		FtpResponse response = SendCommand("USER", name);

		if (response.IsOk())
		{
			response = SendCommand("PASS", password);
		}

		return response;
	}

	FtpResponse Ftp::Disconnect()
	{
		// Send the exit command.
		FtpResponse response = SendCommand("QUIT");

		if (response.IsOk())
		{
			m_commandSocket.Disconnect();
		}

		return response;
	}

	FtpResponse Ftp::KeepAlive()
	{
		return SendCommand("NOOP");
	}

	FtpResponseDirectory Ftp::GetWorkingDirectory()
	{
		return FtpResponseDirectory(SendCommand("PWD"));
	}

	FtpResponseListing Ftp::GetDirectoryListing(const std::string &directory)
	{
		// Open a data channel on default port (20) using ASCII transfer mode.
		std::ostringstream directoryData;
		FtpDataChannel data(*this);
		FtpResponse response = data.Open(FtpDataChannel::Mode::Ascii);

		if (response.IsOk())
		{
			// Tell the server to send us the listing.
			response = SendCommand("NLST", directory);

			if (response.IsOk())
			{
				// Receive the listing.
				data.Receive(directoryData);

				// Get the response from the server.
				response = GetResponse();
			}
		}

		return FtpResponseListing(response, directoryData.str());
	}

	FtpResponse Ftp::ChangeDirectory(const std::string &directory)
	{
		return SendCommand("CWD", directory);
	}

	FtpResponse Ftp::ParentDirectory()
	{
		return SendCommand("CDUP");
	}

	FtpResponse Ftp::CreateRemoteDirectory(const std::string &name)
	{
		return SendCommand("MKD", name);
	}

	FtpResponse Ftp::DeleteRemoteDirectory(const std::string &name)
	{
		return SendCommand("RMD", name);
	}

	FtpResponse Ftp::RenameRemoteFile(const std::string &file, const std::string &newName)
	{
		FtpResponse response = SendCommand("RNFR", file);

		if (response.IsOk())
		{
			response = SendCommand("RNTO", newName);
		}

		return response;
	}

	FtpResponse Ftp::DeleteRemoteFile(const std::string &name)
	{
		return SendCommand("DELE", name);
	}

	FtpResponse Ftp::Download(const std::string &remoteFile, const std::string &localPath, const FtpDataChannel::Mode &mode)
	{
		// Open a data channel using the given transfer mode.
		FtpDataChannel data(*this);
		FtpResponse response = data.Open(mode);

		if (response.IsOk())
		{
			// Tell the server to start the transfer.
			response = SendCommand("RETR", remoteFile);

			if (response.IsOk())
			{
				// Extract the filename from the file path
				std::string filename = remoteFile;
				std::string::size_type pos = filename.find_last_of("/\\");

				if (pos != std::string::npos)
				{
					filename = filename.substr(pos + 1);
				}

				// Make sure the destination path ends with a slash.
				std::string path = localPath;

				if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
				{
					path += "/";
				}

				// Create the file and truncate it if necessary.
				std::ofstream file((path + filename).c_str(), std::ios_base::binary | std::ios_base::trunc);

				if (!file)
				{
					return FtpResponse(FtpResponse::Status::InvalidFile);
				}

				// Receive the file data.
				data.Receive(file);

				// Close the file.
				file.close();

				// Get the response from the server.
				response = GetResponse();

				// If the download was unsuccessful, delete the partial file.
				if (!response.IsOk())
				{
					std::remove((path + filename).c_str());
				}
			}
		}

		return response;
	}

	FtpResponse Ftp::Upload(const std::string &localFile, const std::string &remotePath, const FtpDataChannel::Mode &mode, const bool &append)
	{
		// Get the contents of the file to send.
		std::ifstream file(localFile.c_str(), std::ios_base::binary);

		if (!file)
		{
			return FtpResponse(FtpResponse::Status::InvalidFile);
		}

		// Extract the filename from the file path.
		std::string filename = localFile;
		std::string::size_type pos = filename.find_last_of("/\\");

		if (pos != std::string::npos)
		{
			filename = filename.substr(pos + 1);
		}

		// Make sure the destination path ends with a slash.
		std::string path = remotePath;

		if (!path.empty() && (path[path.size() - 1] != '\\') && (path[path.size() - 1] != '/'))
		{
			path += "/";
		}

		// Open a data channel using the given transfer mode.
		FtpDataChannel data(*this);
		FtpResponse response = data.Open(mode);

		if (response.IsOk())
		{
			// Tell the server to start the transfer.
			response = SendCommand(append ? "APPE" : "STOR", path + filename);

			if (response.IsOk())
			{
				// Send the file data.
				data.Send(file);

				// Get the response from the server.
				response = GetResponse();
			}
		}

		return response;
	}

	FtpResponse Ftp::SendCommand(const std::string &command, const std::string &parameter)
	{
		// Build the command string.
		std::string commandStr;

		if (!parameter.empty())
		{
			commandStr = command + " " + parameter + "\r\n";
		}
		else
		{
			commandStr = command + "\r\n";
		}

		// Send it to the server.
		if (m_commandSocket.Send(commandStr.c_str(), commandStr.length()) != Socket::Status::Done)
		{
			return FtpResponse(FtpResponse::Status::ConnectionClosed);
		}

		// Get the response.
		return GetResponse();
	}

	FtpResponse Ftp::GetResponse()
	{
		// We'll use a variable to keep track of the last valid code.
		// It is useful in case of multi-lines responses, because the end of such a response will start by the same code.
		uint32_t lastCode = 0;
		bool isInsideMultiline = false;
		std::string message;

		for (;;)
		{
			// Receive the response from the server.
			char buffer[1024];
			std::size_t length;

			if (m_receiveBuffer.empty())
			{
				if (m_commandSocket.Receive(buffer, sizeof(buffer), length) != Socket::Status::Done)
				{
					return FtpResponse(FtpResponse::Status::ConnectionClosed);
				}
			}
			else
			{
				std::copy(m_receiveBuffer.begin(), m_receiveBuffer.end(), buffer);
				length = m_receiveBuffer.size();
				m_receiveBuffer.clear();
			}

			// There can be several lines inside the received buffer, extract them all.
			std::istringstream in(std::string(buffer, length), std::ios_base::binary);

			while (in)
			{
				// Try to extract the code.
				uint32_t code;

				if (in >> code)
				{
					// Extract the separator.
					char separator;
					in.get(separator);

					// The '-' character means a multiline response.
					if ((separator == '-') && !isInsideMultiline)
					{
						// Set the multiline flag.
						isInsideMultiline = true;

						// Keep track of the code.
						if (lastCode == 0)
						{
							lastCode = code;
						}

						// Extract the line.
						std::getline(in, message);

						// Remove the ending '\r' (all lines are terminated by "\r\n").
						message.erase(message.length() - 1);
						message = separator + message + "\n";
					}
					else
					{
						// We must make sure that the code is the same, otherwise it means we haven't reached the end of the multiline response.
						if ((separator != '-') && ((code == lastCode) || (lastCode == 0)))
						{
							// Extract the line.
							std::string line;
							std::getline(in, line);

							// Remove the ending '\r' (all lines are terminated by "\r\n").
							line.erase(line.length() - 1);

							// Append it to the message.
							if (code == lastCode)
							{
								std::ostringstream out;
								out << code << separator << line;
								message += out.str();
							}
							else
							{
								message = separator + line;
							}

							// Save the remaining data for the next time getResponse() is called.
							m_receiveBuffer.assign(buffer + static_cast<std::size_t>(in.tellg()),
							                       length - static_cast<std::size_t>(in.tellg()));

							// Return the response code and message.
							return FtpResponse(static_cast<FtpResponse::Status>(code), message);
						}
						// The line we just read was actually not a response, only a new part of the current multiline response.

						// Extract the line.
						std::string line;
						std::getline(in, line);

						if (!line.empty())
						{
							// Remove the ending '\r' (all lines are terminated by "\r\n").
							line.erase(line.length() - 1);

							// Append it to the current message.
							std::ostringstream out;
							out << code << separator << line << "\n";
							message += out.str();
						}
					}
				}
				else if (lastCode != 0)
				{
					// It seems we are in the middle of a multiline response.

					// Clear the error bits of the stream.
					in.clear();

					// Extract the line.
					std::string line;
					std::getline(in, line);

					if (!line.empty())
					{
						// Remove the ending '\r' (all lines are terminated by "\r\n").
						line.erase(line.length() - 1);

						// Append it to the current message.
						message += line + "\n";
					}
				}
				else
				{
					// Error: cannot extract the code, and we are not in a multiline response.
					return FtpResponse(FtpResponse::Status::InvalidResponse);
				}
			}
		}

		// We never reach there.
	}
}
