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

		std::string m_name;
		std::string m_content;

		YamlSection(YamlSection *parent, const std::string &name, const std::string &content) :
			m_parent(parent),
			m_children(std::vector<std::unique_ptr<YamlSection>>()),
			m_name(name),
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

		static void Convert(const YamlSection *source, Metadata *parent);

		static void AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation);
	};
}
