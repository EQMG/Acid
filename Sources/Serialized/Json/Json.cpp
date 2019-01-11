#include "Json.hpp"

#include "Helpers/String.hpp"
#include "JsonSection.hpp"

namespace acid
{
	Json::Json() :
		Metadata("", "")
	{
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

				currentSection->SetContent(currentSection->GetContent() + summation.str());
				summation.str(std::string());

				auto section = new JsonSection(currentSection, name, "");
				currentSection->AddChild(section);
				currentSection = section;
			}
			else if (c == '}' || c == ']')
			{
				currentSection->SetContent(currentSection->GetContent() + summation.str());
				summation.str(std::string());

				if (currentSection->GetParent() != nullptr)
				{
					currentSection = currentSection->GetParent();
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

	void Json::Clear()
	{
		ClearChildren();
	}
}
