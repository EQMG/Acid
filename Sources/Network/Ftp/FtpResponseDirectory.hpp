#pragma once

#include "FtpResponse.hpp"

namespace acid
{
	///
	/// \brief Specialization of FTP response returning a directory.
	///
	class FtpResponseDirectory :
		public FtpResponse
	{
	private:
		std::string m_directory; /// Directory extracted from the response message.
	public:
		///
		/// \brief Default constructor.
		///
		/// \param response Source response.
		///
		FtpResponseDirectory(const FtpResponse &response);

		///
		/// \brief Get the directory returned in the response.
		///
		/// \return Directory name.
		///
		const std::string &GetDirectory() const;
	};
}
