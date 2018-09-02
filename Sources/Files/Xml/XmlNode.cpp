#include "XmlNode.hpp"

namespace acid
{
	XmlNode::XmlNode(XmlNode *parent, const std::string &attributes, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<XmlNode *>()),
		m_attributes(attributes),
		m_content(content)
	{
	}

	XmlNode::~XmlNode()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
	}

	void XmlNode::AppendData(const std::shared_ptr<LoadedValue> &loadedValue, std::stringstream &builder, const int &indentation)
	{
		std::stringstream indents;

		for (int i = 0; i < indentation; i++)
		{
			indents << "\t";
		}

		std::stringstream nameAttributes;
		nameAttributes << loadedValue->GetName();

		for (auto &attribute : loadedValue->GetAttributes())
		{
			nameAttributes << " " << attribute.first << "=\"" << attribute.second << "\"";
		}

		std::string nameAndAttribs = FormatString::Trim(nameAttributes.str());

		builder << indents.str();

		if (loadedValue->GetName()[0] == '?')
		{
			builder << "<" << nameAndAttribs << "?>\n";

			for (auto &child : loadedValue->GetChildren())
			{
				AppendData(child, builder, indentation);
			}

			return;
		}

		if (loadedValue->GetChildren().empty() && loadedValue->GetValue().empty())
		{
			builder << "<" << nameAndAttribs << "/>\n";
			return;
		}

		builder << "<" << nameAndAttribs << ">" << loadedValue->GetValue();

		if (!loadedValue->GetChildren().empty())
		{
			builder << "\n";

			for (auto &child : loadedValue->GetChildren())
			{
				AppendData(child, builder, indentation + 1);
			}

			builder << indents.str();
		}

		builder << "</" << loadedValue->GetName() << ">\n";
	}

	std::shared_ptr<LoadedValue> &XmlNode::Convert(const XmlNode &source, std::shared_ptr<LoadedValue> &parent, const bool &isTopSection)
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
			thisValue = std::make_shared<LoadedValue>(name, source.m_content, parseAttributes);
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
