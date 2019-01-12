#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Json :
		public Metadata
	{
	public:
		Json();

		explicit Json(Metadata *metadata);

		void Load(const std::string &data) override;

		std::string Write() const override;
	private:
		void AddChildren(const Metadata *source, Metadata *destination);
	};
}
