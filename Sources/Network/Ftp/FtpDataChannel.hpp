#pragma once

#include <sstream>
#include <fstream>
#include "Engine/Exports.hpp"
#include "Network/Tcp/TcpSocket.hpp"

namespace acid
{
	class Ftp;

	class FtpResponse;

	///
	/// \brief Enumeration of transfer modes.
	///
	enum FtpTransferMode
	{
		FTP_MODE_BINARY = 0, /// Binary mode (file is transfered as a sequence of bytes).
		FTP_MODE_ASCII = 1, /// Text mode using ASCII encoding.
		FTP_MODE_EBCDIC = 2 /// Text mode using EBCDIC encoding.
	};

	///
	/// \brief Utility class for exchanging datas with the server on the data channel.
	///
	class ACID_EXPORT FtpDataChannel
	{
	private:
		Ftp &m_ftp; /// Reference to the owner Ftp instance.
		TcpSocket m_dataSocket; /// Socket used for data transfers.
	public:
		FtpDataChannel(Ftp &owner);

		FtpResponse Open(FtpTransferMode mode);

		void Send(std::istream &stream);

		void Receive(std::ostream &stream);
	};
}