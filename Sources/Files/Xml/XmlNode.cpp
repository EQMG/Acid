#include "XmlNode.hpp"

namespace fl
{
	XmlNode::XmlNode(std::shared_ptr<XmlNode> parent, const std::string &attributes, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<std::shared_ptr<XmlNode>>()),
		m_attributes(attributes),
		m_content(content)
	{
	}

	XmlNode::~XmlNode()
	{
	}

	void XmlNode::Clear()
	{
		m_parent = nullptr;

		for (auto &child : m_children)
		{
			child->Clear();
		}

		m_children.clear();
	}

	void XmlNode::AppendData(LoadedValue *loadedValue, std::string &data, const int &indentation, const bool &end)
	{
		// if has children, start <name attribs>...</name>
		// else, <name attribs/>
	}

	LoadedValue *XmlNode::Convert(const XmlNode &source, LoadedValue *parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		int firstSpace = FormatString::FindCharPos(source.m_attributes, ' ');
		std::string name = FormatString::Substring(source.m_attributes, 0, firstSpace);
		name = FormatString::Trim(name);
		std::string attributes = FormatString::Substring(source.m_attributes, firstSpace + 1, source.m_attributes.size());
		attributes = FormatString::Trim(attributes);

		if (attributes[attributes.size() - 1] == '/' || attributes[attributes.size() - 1] == '?')
		{
			attributes.pop_back();
		}

		attributes = FormatString::Trim(attributes);

		std::map<std::string, std::string> parseAttributes = {};

		if (!attributes.empty())
		{
			// TODO: Split attributes string.
		//	fprintf_s(stdout, "%s\n", attributes.c_str());
		}

		if (!isTopSection)
		{
			thisValue = new LoadedValue(parent, name, source.m_content, parseAttributes);
			parent->GetChildren().emplace_back(thisValue);
		}
		else
		{
			parent->SetName(name);
			parent->SetValue(source.m_content);
			parent->SetAttributes(parseAttributes);
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
