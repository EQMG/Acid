#include "Xml.hpp"

#include "XmlNode.hpp"

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

		XmlNode *currentSection = nullptr;
		std::stringstream summation;
		bool end = false;

		for (auto it = data.begin(); it != data.end(); ++it)
		{
			if (*it == '<')
			{
				if (*(it + 1) == '?') // Prolog.
				{
					currentSection = new XmlNode(nullptr, "", "");
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

		if (currentSection != nullptr)
		{
			XmlNode::Convert(*currentSection, m_root.get(), 0);

			if (!currentSection->m_children.empty())
			{
				XmlNode::Convert(*currentSection->m_children[0], this, 1);
			}
		}
	}

	std::string Xml::Write() const
	{
		std::stringstream data;
		XmlNode::AppendData(*m_root, data, 0);
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
}
