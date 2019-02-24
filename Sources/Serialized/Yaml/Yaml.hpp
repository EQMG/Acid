#pragma once

#include <utility>
#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT Yaml :
		public Metadata
	{
	public:
		class Section :
			public NonCopyable
		{
		public:
			Section *m_parent;
			std::vector<std::unique_ptr<Section>> m_children;

			uint32_t m_indentation;
			std::string m_content;

			Section(Section *parent, std::string content, const uint32_t &indentation) :
				m_parent(parent),
				m_indentation(indentation),
				m_content(std::move(content))
			{
			}
		};

		Yaml();

		explicit Yaml(Metadata *metadata);

		void Load(const std::string &data) override;

		std::string Write() const override;
	private:
		static void AddChildren(const Metadata *source, Metadata *destination);

		static void Convert(const Section *source, Metadata *parent, const bool &isTopSection = true);

		static void AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation);
	};
}
