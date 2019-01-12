#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Xml :
		public Metadata
	{
	private:
		std::unique_ptr<Metadata> m_root;
	public:
		explicit Xml(const std::string &rootName);

		Xml(const std::string &rootName, Metadata *metadata);

		void Load(const std::string &data) override;

		std::string Write() const override;
	private:
		void AddChildren(const Metadata *source, Metadata *destination);
	};
}
