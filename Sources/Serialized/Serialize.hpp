#pragma once

#include <string>
#include "Metadata.hpp"

namespace acid
{
	enum EncodeProps
	{
		PROP_NONE = 0x00000000,
		PROP_NAME = 0x00000001,
		PROP_VALUE = 0x00000002,
		PROP_CHILDREN = 0x00000004,
		PROP_ATTRIBUTES = 0x00000008
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
