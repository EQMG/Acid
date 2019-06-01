#pragma once

#include "FtpResponse.hpp"

namespace acid
{
/**
 * @brief Specialization of FTP response returning a filename listing.
 **/
class ACID_EXPORT FtpResponseListing :
	public FtpResponse
{
public:
	/**
	 * Default constructor.
	 * @param response Source response. 
	 * @param data Data containing the raw listing. 
	 **/
	FtpResponseListing(const FtpResponse &response, const std::string &data);

	/**
	 * Return the array of directory/file names.
	 * @return Array containing the requested listing. 
	 **/
	const std::vector<std::string> &GetListing() const { return m_listing; }

private:
	/// Directory/file names extracted from the data.
	std::vector<std::string> m_listing;
};
}
