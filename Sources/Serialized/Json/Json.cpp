#include "Json.hpp"

#include <utility>
#include "Helpers/String.hpp"

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
		ClearAttributes();

		auto topSection = std::make_unique<Section>(nullptr, "", "");
		Section *currentSection = nullptr;
		std::stringstream summation;

		for (const char &c : data)
		{
			if (c == '{' || c == '[')
			{
				if (currentSection == nullptr)
				{
					currentSection = topSection.get();
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

				auto section = new Section(currentSection, name, "");
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

		Convert(topSection.get(), this, true);
	}

	std::string Json::Write() const
	{
		std::stringstream data;
		AppendData(this, data, 0);
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

	void Json::Convert(const Section *source, Metadata *parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		if (!isTopSection)
		{
			thisValue = new Metadata(source->m_name, "");
			parent->AddChild(thisValue);
		}

		auto contentSplit = String::Split(source->m_content, ",", true);

		for (const auto &data : contentSplit)
		{
			std::string name = "";
			std::string value = data;

			if (String::Contains(data, ":"))
			{
				name = String::Trim(data.substr(0, data.find(':')));
				value = String::Trim(String::ReplaceFirst(value, name, ""));
				value = String::Trim(value.erase(0, 1));
				name = name.substr(1, name.size() - 2);
			}

			if (value.empty())
			{
				continue;
			}

			if (String::StartsWith(name, "_"))
			{
				name = name.erase(0, 1);

				if (!value.empty())
				{
					value = value.substr(1, value.size() - 2);
				}

				thisValue->AddAttribute(name, value);
			}
			else
			{
				auto newChild = new Metadata(name, value);
				thisValue->AddChild(newChild);
			}
		}

		for (const auto &child : source->m_children)
		{
			Convert(child.get(), thisValue, false);
		}
	}

	void Json::AppendData(const Metadata *source, std::stringstream &builder, const int32_t &indentation, const bool &end)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		char openBrace = '{';
		char closeBrace = '}';

		for (const auto &child : source->GetChildren())
		{
			if (child->GetName().empty())
			{
				openBrace = '[';
				closeBrace = ']';
				break;
			}
		}

		builder << indents.str();

		if (source->GetName().empty())
		{
			builder << openBrace << "\n";
		}
		else if (source->GetValue().empty())
		{
			builder << "\"" << source->GetName() << "\": " << openBrace << "\n";
		}
		else
		{
			builder << "\"" << source->GetName() << "\": " << source->GetValue();

			if (!(end && source->GetAttributes().empty()))
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (const auto &attribute : source->GetAttributes())
		{
			builder << indents.str() << "  \"_" << attribute.first + "\": \"" << attribute.second << "\"";

			if (!(end && source->GetChildren().empty()))
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (const auto &child : source->GetChildren())
		{
			AppendData(child.get(), builder, indentation + 1, child == source->GetChildren().back());
		}

		if (source->GetValue().empty())
		{
			builder << indents.str();

			if (end || indentation == 0)
			{
				builder << closeBrace << "\n";
			}
			else
			{
				builder << closeBrace << ",\n";
			}
		}
	}
}
