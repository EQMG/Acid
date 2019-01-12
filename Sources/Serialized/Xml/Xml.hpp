#pragma once

#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT XmlNode :
		public NonCopyable
	{
	public:
		XmlNode *m_parent;
		std::vector<std::unique_ptr<XmlNode>> m_children;

		std::string m_attributes;
		std::string m_content;

		XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content) :
			m_parent(parent),
			m_children(std::vector<std::unique_ptr<XmlNode>>()),
			m_attributes(attributes),
			m_content(content)
		{
		}
	};

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

		static Metadata *Convert(const XmlNode &source, Metadata *parent, const uint32_t &depth);

		static void AppendData(const Metadata *source, std::stringstream &builder, const int32_t &indentation);
	};
}
