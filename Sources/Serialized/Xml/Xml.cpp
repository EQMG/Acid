#include "Xml.hpp"

#include "XmlNode.hpp"

namespace acid
{
	Xml::Xml() :
		Metadata("?xml", "", std::map<std::string, std::string>{{}, {"encoding", "utf-8"}})
	{
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
					currentSection->SetContent(currentSection->GetContent() + summation.str());
					end = true;
				}
				else // Start tag.
				{
					auto section = new XmlNode(currentSection, "", "");
					currentSection->AddChild(section);
					currentSection = section;
				}

				summation.str(std::string());
			}
			else if (*it == '>')
			{
				if (!end)
				{
					currentSection->SetAttributes(currentSection->GetAttributes() + summation.str());
				}

				summation.str(std::string());

				if (end || *(it - 1) == '/') // End tag.
				{
					end = false;

					if (currentSection->GetParent() != nullptr)
					{
						currentSection = currentSection->GetParent();
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
			XmlNode::Convert(*currentSection, this, true);
		}
	}

	std::string Xml::Write() const
	{
		std::stringstream data;
		XmlNode::AppendData(*this, data, 0);
		return data.str();
	}

	void Xml::Clear()
	{
		ClearChildren();
		ClearAttributes();
		AddAttribute("version",  "1.0");
		AddAttribute("encoding", "utf-8");
	}
}
