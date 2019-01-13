#include <Engine/Log.hpp>
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

		auto topSection = std::make_unique<YamlSection>(nullptr, "", "");
		YamlSection *currentSection = topSection.get();
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
			bool arrayElement = false;
			bool comment = false;

			for (const auto &c : line)
			{
				if (c == ' ')
				{
					indentation++;
				}
				else if (c == '-')
				{
					arrayElement = true;
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

			Log::Out("'%s': %i, %s, ", line.c_str(), indentation, arrayElement ? "true" : "false");

			if (indentation != lastIndentation && !arrayElement)
			{
				auto name = String::Trim(line.substr(0, line.find(':')));
				auto value = String::Trim(String::ReplaceFirst(line, name, ""));
				value = String::Trim(value.erase(0, 1));
				Log::Out("%s, %s, ", name.c_str(), value.c_str());

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
				else
				{
					auto section = new YamlSection(currentSection, name, value);
					currentSection->m_children.emplace_back(section);

					if (indentation > lastIndentation)
					{
						currentSection = section;
					}
				}

				lastIndentation = indentation;
			}


			Log::Out("\n");
		}

		Convert(topSection.get(), this);
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

	void Yaml::Convert(const YamlSection *source, Metadata *parent)
	{
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
