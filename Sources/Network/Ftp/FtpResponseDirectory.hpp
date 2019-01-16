#pragma once

#include "FtpResponse.hpp"

namespace acid
{
	/// <summary>
	/// Specialization of FTP response returning a directory.
	/// </summary>
	class FtpResponseDirectory :
		public FtpResponse
	{
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="response"> Source response. </param>
		explicit FtpResponseDirectory(const FtpResponse &response);

		/// <summary>
		/// Get the directory returned in the response.
		/// </summary>
		/// <returns> Directory name. </returns>
		const std::string &GetDirectory() const { return m_directory; }
	private:
		/// Directory extracted from the response message.
		std::string m_directory;
	};
}
