#include "FtpDataChannel.hpp"

#include <sstream>
#include "Engine/Log.hpp"
#include "Ftp.hpp"

namespace acid
{
	FtpDataChannel::FtpDataChannel(Ftp &owner) :
		m_ftp(owner)
	{
	}

	FtpResponse FtpDataChannel::Open(FtpTransferMode mode)
	{
		// Open a data connection in active mode (we connect to the server).
		FtpResponse response = m_ftp.SendCommand("PASV");

		if (response.IsOk())
		{
			// Extract the connection address and port from the response
			std::string::size_type begin = response.GetFullMessage().find_first_of("0123456789");

			if (begin != std::string::npos)
			{
				uint8_t data[6] = {0, 0, 0, 0, 0, 0};
				std::string str = response.GetFullMessage().substr(begin);
				std::size_t index = 0;

				for (int i = 0; i < 6; ++i)
				{
					// Extract the current number.
					while (isdigit(str[index]))
					{
						data[i] = data[i] * 10 + (str[index] - '0');
						index++;
					}

					// Skip separator.
					index++;
				}

				// Reconstruct connection port and address.
				unsigned short port = data[4] * 256 + data[5];
				IpAddress address(static_cast<uint8_t>(data[0]),
				                  static_cast<uint8_t>(data[1]),
				                  static_cast<uint8_t>(data[2]),
				                  static_cast<uint8_t>(data[3]));

				// Connect the data channel to the server.
				if (m_dataSocket.Connect(address, port) == SOCKET_STATUS_DONE)
				{
					// Translate the transfer mode to the corresponding FTP parameter.
					std::string modeStr;

					switch (mode)
					{
						case FTP_MODE_BINARY:
							modeStr = "I";
							break;
						case FTP_MODE_ASCII:
							modeStr = "A";
							break;
						case FTP_MODE_EBCDIC:
							modeStr = "E";
							break;
					}

					// Set the transfer mode.
					response = m_ftp.SendCommand("TYPE", modeStr);
				}
				else
				{
					// Failed to connect to the server.
					response = FtpResponse(FTP_RESPONSE_CONNECTION_FAILED);
				}
			}
		}

		return response;
	}

	void FtpDataChannel::Receive(std::ostream &stream)
	{
		// Receive data.
		char buffer[1024];
		std::size_t received;

		while (m_dataSocket.Receive(buffer, sizeof(buffer), received) == SOCKET_STATUS_DONE)
		{
			stream.write(buffer, static_cast<std::streamsize>(received));

			if (!stream.good())
			{
				Log::Error("FTP Error: Writing to the file has failed\n");
				break;
			}
		}

		// Close the data socket.
		m_dataSocket.Disconnect();
	}

	void FtpDataChannel::Send(std::istream &stream)
	{
		// Send data.
		char buffer[1024];
		std::size_t count;

		for (;;)
		{
			// Read some data from the stream.
			stream.read(buffer, sizeof(buffer));

			if (!stream.good() && !stream.eof())
			{
				Log::Error("FTP Error: Reading from the file has failed\n");
				break;
			}

			count = static_cast<std::size_t>(stream.gcount());

			if (count > 0)
			{
				// We could read more data from the stream: send them.
				if (m_dataSocket.Send(buffer, count) != SOCKET_STATUS_DONE)
				{
					break;
				}
			}
			else
			{
				// No more data: exit the loop.
				break;
			}
		}

		// Close the data socket.
		m_dataSocket.Disconnect();
	}
}
