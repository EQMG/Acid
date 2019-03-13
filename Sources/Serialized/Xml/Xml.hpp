#pragma once

#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
class ACID_EXPORT Xml : public Metadata
{
  public:
	class ACID_EXPORT Node : public NonCopyable
	{
	  public:
		Node* m_parent;
		std::vector<std::unique_ptr<Node>> m_children;

		std::string m_attributes;
		std::string m_content;

		Node(Node* parent, std::string attributes, std::string content) : m_parent(parent), m_attributes(std::move(attributes)), m_content(std::move(content)) {}
	};

	explicit Xml(const std::string& rootName);

	Xml(const std::string& rootName, Metadata* metadata);

	void Load(std::istream* inStream) override;

	void Write(std::ostream* outStream) const override;

  private:
	static void AddChildren(const Metadata* source, Metadata* destination);

	static void Convert(const Node* source, Metadata* parent, const uint32_t& depth);

	static void AppendData(const Metadata* source, std::ostream* outStream, const int32_t& indentation);
};
}
