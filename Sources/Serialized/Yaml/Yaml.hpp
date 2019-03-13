#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class ACID_EXPORT Yaml : public Metadata
{
  public:
	class Section : public NonCopyable
	{
	  public:
		Section* m_parent;
		std::vector<std::unique_ptr<Section>> m_children;

		uint32_t m_indentation;
		uint32_t m_arrayLevels;
		std::string m_content;

		Section(Section* parent, std::string content, const uint32_t& indentation, const uint32_t& arrayLevels) : m_parent(parent), m_indentation(indentation), m_arrayLevels(arrayLevels), m_content(std::move(content)) {}
	};

	Yaml();

	explicit Yaml(Metadata* metadata);

	void Load(std::istream* inStream) override;

	void Write(std::ostream* outStream) const override;

  private:
	static void AddChildren(const Metadata* source, Metadata* destination);

	static void Convert(const Section* source, Metadata* parent, const bool& isTopSection = true);

	static void AppendData(const Metadata* source, const Metadata* parent, std::ostream* outStream, const int32_t& indentation);
};
}
