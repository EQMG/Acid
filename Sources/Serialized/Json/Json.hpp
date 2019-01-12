#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class JsonSection :
		public NonCopyable
	{
	public:
		JsonSection *m_parent;
		std::vector<std::unique_ptr<JsonSection>> m_children;

		std::string m_name;
		std::string m_content;

		JsonSection(JsonSection *parent, const std::string &name, const std::string &content) :
			m_parent(parent),
			m_children(std::vector<std::unique_ptr<JsonSection>>()),
			m_name(name),
			m_content(content)
		{
		}
	};

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

		static Metadata *Convert(const JsonSection &source, Metadata *parent, const bool &isTopSection = true);

		static void AppendData(const Metadata *source, std::stringstream &builder, const int32_t &indentation, const bool &end = false);
	};
}
