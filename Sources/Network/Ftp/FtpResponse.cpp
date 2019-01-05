#include "FtpResponse.hpp"

namespace acid
{
	FtpResponse::FtpResponse(const FtpResponseStatus &code, const std::string &message) :
		m_status(code),
		m_message(message)
	{
	}

	bool FtpResponse::IsOk() const
	{
		return m_status < 400;
	}

	FtpResponseStatus FtpResponse::GetStatus() const
	{
		return m_status;
	}

	std::string FtpResponse::GetFullMessage() const
	{
		return m_message;
	}
}
