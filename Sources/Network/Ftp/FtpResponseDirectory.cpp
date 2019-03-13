#include "FtpResponseDirectory.hpp"

namespace acid
{
FtpResponseDirectory::FtpResponseDirectory(const FtpResponse& response) : FtpResponse(response)
{
	if(IsOk())
		{
			// Extract the directory from the server response
			std::string::size_type begin = GetFullMessage().find('"', 0);
			std::string::size_type end = GetFullMessage().find('"', begin + 1);
			m_directory = GetFullMessage().substr(begin + 1, end - begin - 1);
		}
}
}
