#pragma once

#include <string>
#include "Metadata.hpp"

namespace acid
{
	enum EncodeProps
	{
		ENCODE_PROP_NONE = 0x00000000,
		ENCODE_PROP_NAME = 0x00000001,
		ENCODE_PROP_VALUE = 0x00000002,
		ENCODE_PROP_CHILDREN = 0x00000004,
		ENCODE_PROP_ATTRIBUTES = 0x00000008
	};
	typedef uint32_t EncodePropsFlags;

	class ACID_EXPORT Serialize
	{
	public:
		static std::string Encode(const Metadata &metadata);

		static Metadata Decode(const std::string &value);
	private:
		static void EncodeStream(const Metadata &metadata, std::stringstream &ss);
	};
}
