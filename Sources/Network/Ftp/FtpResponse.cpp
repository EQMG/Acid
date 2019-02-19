#include "FtpResponse.hpp"

namespace acid
{
	FtpResponse::FtpResponse(const Status &code, const std::string &message) :
		m_status(code),
		m_message(message)
	{
	}

	bool FtpResponse::IsOk() const
	{
		return static_cast<uint32_t>(m_status) < 400;
	}
}
