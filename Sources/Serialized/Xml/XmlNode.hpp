#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "Helpers/NonCopyable.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT XmlNode :
		public NonCopyable
	{
	private:
		friend class Xml;
		XmlNode *m_parent;
		std::vector<std::unique_ptr<XmlNode>> m_children;

		std::string m_attributes;
		std::string m_content;
	public:
		XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content);

		static void AppendData(const Metadata &source, std::stringstream &builder, const int32_t &indentation);

		static Metadata *Convert(const XmlNode &source, Metadata *parent, const uint32_t &depth);
	};
}
