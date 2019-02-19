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
	/// Utility class for exchanging datas with the server on the data channel.
	/// </summary>
	class ACID_EXPORT FtpDataChannel
	{
	public:
		/// <summary>
		/// Enumeration of transfer modes.
		/// </summary>
		enum class Mode
		{
			/// Binary mode (file is transfered as a sequence of bytes).
			Binary,
			/// Text mode using ASCII encoding.
			Ascii,
			/// Text mode using EBCDIC encoding.
			Ebcdic
		};

		explicit FtpDataChannel(Ftp &owner);

		FtpResponse Open(const Mode &mode);

		void Send(std::istream &stream);

		void Receive(std::ostream &stream);

	private:
		/// Reference to the owner Ftp instance.
		Ftp &m_ftp;
		/// Socket used for data transfers.
		TcpSocket m_dataSocket;
	};
}
