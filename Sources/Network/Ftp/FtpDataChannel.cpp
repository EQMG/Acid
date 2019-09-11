#include "FtpDataChannel.hpp"

#include "Ftp.hpp"

namespace acid
{
FtpDataChannel::FtpDataChannel(Ftp &owner) :
	m_ftp{owner}
{
}

FtpResponse FtpDataChannel::Open(const Mode &mode)
{
	// Open a data connection in active mode (we connect to the server).
	auto response = m_ftp.SendCommand("PASV");

	if (response.IsOk())
	{
		// Extract the connection address and port from the response
		auto begin = response.GetFullMessage().find_first_of("0123456789");

		if (begin != std::string::npos)
		{
			uint8_t data[6] = {0, 0, 0, 0, 0, 0};
			auto str = response.GetFullMessage().substr(begin);
			std::size_t index{};

			for (auto &i : data)
			{
				// Extract the current number.
				while (isdigit(str[index]))
				{
					i = i * 10 + (str[index] - '0');
					index++;
				}

				// Skip separator.
				index++;
			}

			// Reconstruct connection port and address.
			auto port = static_cast<uint16_t>(data[4] * 256 + data[5]);
			IpAddress address{static_cast<uint8_t>(data[0]), static_cast<uint8_t>(data[1]), static_cast<uint8_t>(data[2]), static_cast<uint8_t>(data[3])};

			// Connect the data channel to the server.
			if (m_dataSocket.Connect(address, port) == Socket::Status::Done)
			{
				// Translate the transfer mode to the corresponding FTP parameter.
				std::string modeStr;

				switch (mode)
				{
				case Mode::Binary:
					modeStr = "I";
					break;
				case Mode::Ascii:
					modeStr = "A";
					break;
				case Mode::Ebcdic:
					modeStr = "E";
					break;
				}

				// Set the transfer mode.
				response = m_ftp.SendCommand("TYPE", modeStr);
			}
			else
			{
				// Failed to connect to the server.
				response = {FtpResponse::Status::ConnectionFailed};
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

	while (m_dataSocket.Receive(buffer, sizeof(buffer), received) == Socket::Status::Done)
	{
		stream.write(buffer, static_cast<std::streamsize>(received));

		if (!stream.good())
		{
			std::cerr << "FTP Error: Writing to the file has failed\n";
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
			std::cerr << "FTP Error: Reading from the file has failed\n";
			break;
		}

		count = static_cast<std::size_t>(stream.gcount());

		if (count > 0)
		{
			// We could read more data from the stream: send them.
			if (m_dataSocket.Send(buffer, count) != Socket::Status::Done)
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
