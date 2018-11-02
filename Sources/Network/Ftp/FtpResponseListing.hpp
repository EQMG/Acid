#pragma once

#include <vector>
#include "FtpResponse.hpp"

namespace acid
{
	/// <summary>
	/// Specialization of FTP response returning a filename listing.
	/// </summary>
	class FtpResponseListing :
		public FtpResponse
	{
	private:
		/// Directory/file names extracted from the data.
		std::vector<std::string> m_listing;
	public:
		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <param name="response"> Source response. </param>
		/// <param name="data"> Data containing the raw listing. </param>
		FtpResponseListing(const FtpResponse &response, const std::string &data);

		/// <summary>
		/// Return the array of directory/file names.
		/// </summary>
		/// <returns> Array containing the requested listing. </returns>
		const std::vector<std::string> &GetListing() const;
	};
}
