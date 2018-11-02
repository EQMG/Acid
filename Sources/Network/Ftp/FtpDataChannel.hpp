#pragma once

#include <sstream>
#include <fstream>
#include "Engine/Exports.hpp"
#include "Network/Tcp/TcpSocket.hpp"

namespace acid
{
	class Ftp;

	class FtpResponse;

	/// <summary>
	/// Enumeration of transfer modes.
	/// </summary>
	enum FtpTransferMode
	{
		/// Binary mode (file is transfered as a sequence of bytes).
		FTP_MODE_BINARY = 0,
		/// Text mode using ASCII encoding.
		FTP_MODE_ASCII = 1,
		/// Text mode using EBCDIC encoding.
		FTP_MODE_EBCDIC = 2
	};

	/// <summary>
	/// Utility class for exchanging datas with the server on the data channel.
	/// </summary>
	class ACID_EXPORT FtpDataChannel
	{
	private:
		/// Reference to the owner Ftp instance.
		Ftp &m_ftp;
		/// Socket used for data transfers.
		TcpSocket m_dataSocket;
	public:
		FtpDataChannel(Ftp &owner);

		FtpResponse Open(FtpTransferMode mode);

		void Send(std::istream &stream);

		void Receive(std::ostream &stream);
	};
}