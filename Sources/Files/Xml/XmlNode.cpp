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

	void XmlNode::AppendData(LoadedValue *loadedValue, std::string &data, const int &indentation)
	{
		std::string nameAndAttribs = loadedValue->GetName();

		for (auto &attribute : loadedValue->GetAttributes())
		{
			nameAndAttribs += " " + attribute.first + "=\"" + attribute.second + "\"";
		}

		nameAndAttribs = FormatString::Trim(nameAndAttribs);

		std::string indent;

		for (int i = 0; i < indentation; i++)
		{
			indent += "  ";
		}

		data += indent;

		if (loadedValue->GetName()[0] == '?')
		{
			data += "<" + nameAndAttribs + "?>\n";

			for (auto &child : loadedValue->GetChildren())
			{
				AppendData(child, data, indentation);
			}

			return;
		}

		if (loadedValue->GetChildren().empty() && loadedValue->GetValue().empty())
		{
			data += "<" + nameAndAttribs + "/>\n";
			return;
		}

		data += "<" + nameAndAttribs + ">" + loadedValue->GetValue();

		if (!loadedValue->GetChildren().empty())
		{
			data += "\n";

			for (auto &child : loadedValue->GetChildren())
			{
				AppendData(child, data, indentation + 1);
			}

			data += indent;
		}

		data += "</" + loadedValue->GetName() + ">\n";
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
			std::string currentKey = "";
			std::string summation = "";

			for (char &c : attributes)
			{
				switch (c)
				{
				case '"':
				{
					if (currentKey.empty())
					{
						currentKey = summation;
						summation.clear();
						continue;
					}

					parseAttributes.emplace(FormatString::Trim(currentKey), FormatString::Trim(summation));
					currentKey.clear();
					summation.clear();
					break;
				}
				case '=':
				{
					if (!currentKey.empty())
					{
						summation += c;
					}

					break;
				}
				default:
				{
					summation += c;
					break;
				}
				}
			}
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
