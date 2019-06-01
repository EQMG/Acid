#pragma once

#include "FtpResponse.hpp"

namespace acid
{
/**
 * @brief Specialization of FTP response returning a directory.
 **/
class ACID_EXPORT FtpResponseDirectory :
	public FtpResponse
{
public:
	/**
	 * Default constructor.
	 * @param response Source response. 
	 **/
	explicit FtpResponseDirectory(const FtpResponse &response);

	/**
	 * Get the directory returned in the response.
	 * @return Directory name. 
	 **/
	const std::string &GetDirectory() const { return m_directory; }

private:
	/// Directory extracted from the response message.
	std::string m_directory;
};
}
