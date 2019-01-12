#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class Yaml : // ACID_EXPORT
		public Metadata
	{
	public:
		Yaml();

		explicit Yaml(Metadata *metadata);

		void Load(const std::string &data) override;

		std::string Write() const override;
	private:
		void AddChildren(const Metadata *source, Metadata *destination);

		static void AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation);
	};
}
