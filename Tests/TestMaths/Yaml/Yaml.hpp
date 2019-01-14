#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class YamlSection :
		public NonCopyable
	{
	public:
		YamlSection *m_parent;
		std::vector<std::unique_ptr<YamlSection>> m_children;

		uint32_t m_indentation;
		std::string m_content;

		YamlSection(YamlSection *parent, const std::string &content, const uint32_t &indentation) :
			m_parent(parent),
			m_children(std::vector<std::unique_ptr<YamlSection>>()),
			m_indentation(indentation),
			m_content(content)
		{
		}
	};

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

		static void Convert(const YamlSection *source, Metadata *parent, const bool &isTopSection = true);

		static void AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation);
	};
}
