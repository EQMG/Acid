#include "FtpResponseDirectory.hpp"

namespace acid
{
FtpResponseDirectory::FtpResponseDirectory(const FtpResponse &response) :
	FtpResponse(response)
{
	if (IsOk())
	{
		// Extract the directory from the server response
		auto begin = GetFullMessage().find('"', 0);
		auto end = GetFullMessage().find('"', begin + 1);
		m_directory = GetFullMessage().substr(begin + 1, end - begin - 1);
	}
}
}
