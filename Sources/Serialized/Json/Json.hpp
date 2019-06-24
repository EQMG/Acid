#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class ACID_EXPORT Json :
	public Metadata
{
public:
	Json() = default;

	explicit Json(Metadata *metadata);

	void Load(std::istream *inStream) override;

	void Write(std::ostream *outStream, const Format &format = Format::Beautified) const override;

	void Load(const std::string &string);

	std::string Write(const Format &format = Format::Beautified) const;

private:
	enum class Token
	{
		Unknown, String, Number, CroushOpen, CroushClose, BracketOpen, BracketClose, Comma, Colon, Boolean, Null
	};

	static void AddChildren(const Metadata *source, Metadata *destination);

	static Metadata *Convert(Metadata *current, std::vector<std::pair<Token, std::string>> v, const int32_t &i, int32_t &r);

	static void AppendData(const Metadata *source, std::ostream *outStream, const int32_t &indentation, const Format &format, const bool &end = false);
};
}
