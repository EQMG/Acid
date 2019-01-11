#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Xml :
		public Metadata
	{
	public:
		Xml();

		void Load(const std::string &data) override;

		std::string Write() const override;

		void Clear() override;
	};
}
