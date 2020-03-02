#include "FtpResponse.hpp"

namespace acid {
FtpResponse::FtpResponse(Status code, std::string message) :
	status(code),
	message(std::move(message)) {
}

bool FtpResponse::IsOk() const {
	return static_cast<uint32_t>(status) < 400;
}
}
