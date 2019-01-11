#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Json :
		public Metadata
	{
	public:
		Json();

		void Load(const std::string &data) override;

		std::string Write() const override;

		void Clear() override;
	};
}
