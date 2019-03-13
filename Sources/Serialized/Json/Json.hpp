#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class ACID_EXPORT Json : public Metadata
{
  public:
	class Section : public NonCopyable
	{
	  public:
		Section* m_parent;
		std::vector<std::unique_ptr<Section>> m_children;

		std::string m_name;
		std::string m_content;

		Section(Section* parent, std::string name, std::string content) : m_parent(parent), m_name(std::move(name)), m_content(std::move(content)) {}
	};

	Json();

	explicit Json(Metadata* metadata);

	void Load(std::istream* inStream) override;

	void Write(std::ostream* outStream) const override;

  private:
	static void AddChildren(const Metadata* source, Metadata* destination);

	static void Convert(const Section* source, Metadata* parent, const bool& isTopSection = true);

	static void AppendData(const Metadata* source, std::ostream* outStream, const int32_t& indentation, const bool& end = false);
};
}
