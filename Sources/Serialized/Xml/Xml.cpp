#include "Xml.hpp"

namespace acid
{
	Xml::Xml(const std::string &rootName) :
		Metadata(rootName, ""),
		m_root(std::make_unique<Metadata>("?xml", "", std::map<std::string, std::string>{{"version", "1.0"}, {"encoding", "utf-8"}}))
	{
		m_root->AddChild(this);
	}

	Xml::Xml(const std::string &rootName, Metadata *metadata) :
		Metadata(rootName, ""),
		m_root(std::make_unique<Metadata>("?xml", "", std::map<std::string, std::string>{{"version", "1.0"}, {"encoding", "utf-8"}}))
	{
		m_root->AddChild(this);
		AddChildren(metadata, this);
	}

	void Xml::Load(const std::string &data)
	{
		ClearChildren();
		ClearAttributes();

		auto topNode = std::make_unique<XmlNode>(nullptr, "", "");
		XmlNode *currentSection = nullptr;
		std::stringstream summation;
		bool end = false;

		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (*it == '<')
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = topNode.get();
					continue;
				}

				if (*(it + 1) == '/') // End tag.
				{
					currentSection->m_content += summation.str();
					end = true;
				}
				else // Start tag.
				{
					auto section = new XmlNode(currentSection, "", "");
					currentSection->m_children.emplace_back(section);
					currentSection = section;
				}

				summation.str(std::string());
			}
			else if (*it == '>')
			{
				if (!end)
				{
					currentSection->m_attributes += summation.str();
				}

				summation.str(std::string());

				if (end || *(it - 1) == '/') // End tag.
				{
					end = false;

					if (currentSection->m_parent != nullptr)
					{
						currentSection = currentSection->m_parent;
					}
				}
			}
			else if (*it == '\n')
			{
			}
			else
			{
				summation << *it;
			}
		}

		Convert(topNode.get(), m_root.get(), 0);

		if (!topNode->m_children.empty())
		{
			Convert(topNode->m_children[0].get(), this, 1);
		}
	}

	std::string Xml::Write() const
	{
		std::stringstream data;
		AppendData(m_root.get(), data, 0);
		return data.str();
	}

	void Xml::AddChildren(const Metadata *source, Metadata *destination)
	{
		for (const auto &child : source->GetChildren())
		{
			auto created = destination->AddChild(new Metadata(child->GetName(), child->GetValue()));
			AddChildren(child.get(), created);
		}

		for (const auto &attribute : source->GetAttributes())
		{
			destination->AddAttribute(attribute.first, attribute.second);
		}
	}

	void Xml::Convert(const XmlNode *source, Metadata *parent, const uint32_t &depth)
	{
		int32_t firstSpace = String::FindCharPos(source->m_attributes, ' ');
		std::string name = String::Trim(String::Substring(source->m_attributes, 0, firstSpace));
		std::string attributes = String::Substring(source->m_attributes, firstSpace + 1, static_cast<int32_t>(source->m_attributes.size()));
		attributes = String::Trim(attributes);

		if (attributes[attributes.size() - 1] == '/' || attributes[attributes.size() - 1] == '?')
		{
			attributes.pop_back();
		}

		attributes = String::Trim(attributes);

		std::map<std::string, std::string> parseAttributes = {};

		if (!attributes.empty())
		{
			std::string currentKey;
			std::string summation;

			for (const char &c : attributes)
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

						parseAttributes.emplace(String::Trim(currentKey), String::Trim(summation));
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

		auto thisValue = parent;

		if (depth != 0)
		{
			if (depth != 1)
			{
				thisValue = new Metadata(name, source->m_content, parseAttributes);
				parent->AddChild(thisValue);
			}
			else
			{
				thisValue->SetName(name);
				thisValue->SetValue(source->m_content);
				thisValue->SetAttributes(parseAttributes);
			}

			for (const auto &child : source->m_children)
			{
				Convert(child.get(), thisValue, depth + 1);
			}
		}
		else
		{
			parent->SetName(name);
			parent->SetValue(source->m_content);
			parent->SetAttributes(parseAttributes);
		}
	}

	void Xml::AppendData(const Metadata *source, std::stringstream &builder, const int32_t &indentation)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		std::string name = String::ReplaceAll(source->GetName(), " ", "_");

		std::stringstream nameAttributes;
		nameAttributes << name;

		for (const auto &[attributeName, value] : source->GetAttributes())
		{
			nameAttributes << " " << attributeName << "=\"" << value << "\"";
		}

		std::string nameAndAttribs = String::Trim(nameAttributes.str());

		builder << indents.str();

		if (source->GetName()[0] == '?')
		{
			builder << "<" << nameAndAttribs << "?>\n";

			for (const auto &child : source->GetChildren())
			{
				AppendData(child.get(), builder, indentation);
			}

			return;
		}

		if (source->GetChildren().empty() && source->GetValue().empty())
		{
			builder << "<" << nameAndAttribs << "/>\n";
			return;
		}

		builder << "<" << nameAndAttribs << ">" << source->GetString();

		if (!source->GetChildren().empty())
		{
			builder << "\n";

			for (const auto &child : source->GetChildren())
			{
				AppendData(child.get(), builder, indentation + 1);
			}

			builder << indents.str();
		}

		builder << "</" << name << ">\n";
	}
}
