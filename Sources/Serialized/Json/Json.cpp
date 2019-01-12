#include "Json.hpp"

#include "Helpers/String.hpp"
#include "JsonSection.hpp"

namespace acid
{
	Json::Json() :
		Metadata("", "")
	{
	}

	Json::Json(Metadata *metadata) :
		Metadata("", "")
	{
		AddChildren(metadata, this);
	}

	void Json::Load(const std::string &data)
	{
		ClearChildren();

		JsonSection *currentSection = nullptr;
		std::stringstream summation;

		for (const char &c : data)
		{
			if (c == '{' || c == '[')
			{
				if (currentSection == nullptr)
				{
					currentSection = new JsonSection(nullptr, "", "");
					continue;
				}

				std::string name;

				if (!summation.str().empty())
				{
					auto contentSplit = String::Split(summation.str(), "\"");

					if (static_cast<int32_t>(contentSplit.size()) - 2 >= 0)
					{
						name = contentSplit.at(contentSplit.size() - 2);
					}
				}

				currentSection->m_content += summation.str();
				summation.str(std::string());

				auto section = new JsonSection(currentSection, name, "");
				currentSection->m_children.emplace_back(section);
				currentSection = section;
			}
			else if (c == '}' || c == ']')
			{
				currentSection->m_content += summation.str();
				summation.str(std::string());

				if (currentSection->m_parent != nullptr)
				{
					currentSection = currentSection->m_parent;
				}
			}
			else if (c == '\n')
			{
			}
			else
			{
				summation << c;
			}
		}

		if (currentSection != nullptr)
		{
			JsonSection::Convert(*currentSection, this, true);
		}
	}

	std::string Json::Write() const
	{
		std::stringstream data;
		JsonSection::AppendData(*this, data, 0);
		return data.str();
	}

	void Json::AddChildren(const Metadata *source, Metadata *destination)
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
