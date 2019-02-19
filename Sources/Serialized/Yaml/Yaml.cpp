#include "Yaml.hpp"

#include "Helpers/String.hpp"

namespace acid
{
	Yaml::Yaml() :
		Metadata("", "")
	{
	}

	Yaml::Yaml(Metadata *metadata) :
		Metadata("", "")
	{
		AddChildren(metadata, this);
	}

	void Yaml::Load(const std::string &data)
	{
		ClearChildren();
		ClearAttributes();

		auto topSection = std::make_unique<Section>(nullptr, "", 0);
		Section *currentSection = topSection.get();
		uint32_t lastIndentation = 0;

		auto lines = String::Split(data, "\n");

		for (const auto &line : lines)
		{
			// Start marker.
			if (line == "---")
			{
				continue;
			}

			uint32_t indentation = 0;
			bool comment = false;
			//	bool array = false;

			for (const auto &c : line)
			{
				if (c == ' ')
				{
					indentation++;
				}
				else if (c == '-')
				{
					//	array = true;
					indentation += 2;
					break;
				}
				else if (c == '#')
				{
					comment = true;
					break;
				}
				else
				{
					break;
				}
			}

			if (comment)
			{
				continue;
			}

			if (indentation < lastIndentation)
			{
				for (uint32_t i = 0; i < (lastIndentation - indentation) / 2; i++)
				{
					if (currentSection->m_parent != nullptr)
					{
						currentSection = currentSection->m_parent;
					}
				}
			}
			else if (indentation > lastIndentation)
			{
				for (uint32_t i = 0; i < ((indentation - lastIndentation) / 2) - 1; i++)
				{
					auto section = new Section(currentSection, "", lastIndentation + (i * 2));
					currentSection->m_children.emplace_back(section);
					currentSection = section;
				}
			}

			if (indentation - lastIndentation == 2)
			{
				currentSection = currentSection->m_children.back().get();
			}

			/*if (array)
			{
				{
					indentation += 2;
					auto section = new Section(currentSection, "", indentation);
					currentSection->m_children.emplace_back(section);
					currentSection = section;
				}
				auto section = new Section(currentSection, String::Trim(line).erase(0, 1), indentation);
				currentSection->m_children.emplace_back(section);
			}
			else
			{*/
			auto section = new Section(currentSection, String::Trim(line), indentation);
			currentSection->m_children.emplace_back(section);
			//}

			lastIndentation = indentation;
		}

		Convert(topSection.get(), this, true);
	}

	std::string Yaml::Write() const
	{
		std::stringstream data;
		data << "---\n";
		AppendData(this, nullptr, data, 0);
		return data.str();
	}

	void Yaml::AddChildren(const Metadata *source, Metadata *destination)
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

	void Yaml::Convert(const Section *source, Metadata *parent, const bool &isTopSection)
	{
		auto name = String::Trim(source->m_content.substr(0, source->m_content.find(':')));
		auto value = String::Trim(String::ReplaceFirst(source->m_content, name, ""));
		value = String::Trim(value.erase(0, 1));

		auto thisValue = parent;

		if (String::StartsWith(name, "_"))
		{
			name = name.erase(0, 1);
			parent->AddAttribute(name, value);
			return;
		}
		if (!isTopSection)
		{
			thisValue = new Metadata(name, value);
			parent->AddChild(thisValue);
		}

		for (const auto &child : source->m_children)
		{
			Convert(child.get(), thisValue, false);
		}
	}

	void Yaml::AppendData(const Metadata *source, const Metadata *parent, std::stringstream &builder, const int32_t &indentation)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		if (parent != nullptr && !(parent->GetChildren()[0].get() == source && parent->GetName().empty() && parent->GetValue().empty()))
		{
			builder << indents.str();
		}

		if (parent != nullptr && parent->GetValue().empty() && parent->GetChildren()[0]->GetName().empty())
		{
			builder.seekp(-2, std::stringstream::cur);
			builder << "- ";
		}

		if (!source->GetName().empty())
		{
			if (source->GetValue().empty())
			{
				builder << source->GetName() << ": \n";
			}
			else
			{
				builder << source->GetName() << ": " << source->GetValue() << "\n";
			}
		}
		else if (!source->GetValue().empty())
		{
			builder << source->GetValue() << "\n";
		}

		for (const auto &attribute : source->GetAttributes())
		{
			builder << indents.str() << "  _" << attribute.first + ": " << attribute.second << "\n";
		}

		for (const auto &child : source->GetChildren())
		{
			AppendData(child.get(), source, builder, indentation + !source->GetName().empty());
		}
	}
}
