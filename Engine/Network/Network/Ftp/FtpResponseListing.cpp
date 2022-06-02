#include "FtpResponseListing.hpp"

namespace acid {
FtpResponseListing::FtpResponseListing(const FtpResponse &response, const std::string &data) :
	FtpResponse(response) {
	if (IsOk()) {
		// Fill the array of strings
		std::string::size_type lastPos = 0;

		for (auto pos = data.find("\r\n"); pos != std::string::npos; pos = data.find("\r\n", lastPos)) {
			listing.emplace_back(data.substr(lastPos, pos - lastPos));
			lastPos = pos + 2;
		}
	}
}
}
