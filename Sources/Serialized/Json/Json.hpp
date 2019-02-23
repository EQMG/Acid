#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Json :
		public Metadata
	{
	public:
		class Section :
			public NonCopyable
		{
		public:
			Section *m_parent;
			std::vector<std::unique_ptr<Section>> m_children;

			std::string m_name;
			std::string m_content;

			Section(Section *parent, const std::string &name, const std::string &content) :
				m_parent(parent),
				m_children(std::vector<std::unique_ptr<Section>>()),
				m_name(name),
				m_content(content)
			{
			}
		};

		Json();

		explicit Json(Metadata *metadata);

		void Load(const std::string &data) override;

		std::string Write() const override;
	private:
		void AddChildren(const Metadata *source, Metadata *destination);

		static void Convert(const Section *source, Metadata *parent, const bool &isTopSection = true);

		static void AppendData(const Metadata *source, std::stringstream &builder, const int32_t &indentation, const bool &end = false);
	};
}
