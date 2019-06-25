#include "Json.hpp"

#include "Files/Files.hpp"
#include "Helpers/String.hpp"

namespace acid
{
Json::Json(Metadata *metadata)
{
	AddChildren(metadata, this);
}

void Json::Load(std::istream *inStream)
{
	ClearChildren();
	ClearAttributes();

	auto topSection{std::make_unique<Section>(nullptr, "", "")};
	Section *currentSection{};
	std::stringstream summation;

	std::size_t lineNum{};
	std::string linebuf;

	while (inStream->peek() != -1)
	{
		Files::SafeGetLine(*inStream, linebuf);
		lineNum++;

		for (const auto &c : linebuf)
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
					auto contentSplit{String::Split(summation.str(), '\"')};

					if (static_cast<int32_t>(contentSplit.size()) - 2 >= 0)
					{
						name = contentSplit.at(contentSplit.size() - 2);
					}
				}

				currentSection->m_content += summation.str();
				summation.str({});

				auto section{new Section(currentSection, name, "")};
				currentSection->m_children.emplace_back(section);
				currentSection = section;
			}
			else if (c == '}' || c == ']')
			{
				currentSection->m_content += summation.str();
				summation.str({});

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
	}

	Convert(topSection.get(), this, true);
}

void Json::Write(std::ostream *outStream, const Format &format) const
{
	AppendData(this, outStream, 0, format);
}

void Json::Load(const std::string &string)
{
	std::stringstream stream{string};
	Load(&stream);
}

std::string Json::Write(const Format &format) const
{
	std::stringstream stream;
	Write(&stream, format);
	return stream.str();
}

void Json::AddChildren(const Metadata *source, Metadata *destination)
{
	for (const auto &child : source->GetChildren())
	{
		auto created{destination->AddChild(std::make_unique<Metadata>(child->GetName(), child->GetValue()))};
		AddChildren(child.get(), created);
	}

	for (const auto &attribute : source->GetAttributes())
	{
		destination->SetAttribute(attribute.first, attribute.second);
	}
}

void Json::Convert(const Section *source, Metadata *parent, const bool &isTopSection)
{
	auto thisValue{parent};

	if (!isTopSection)
	{
		thisValue = parent->AddChild(std::make_unique<Metadata>(source->m_name));
	}

	auto contentSplit{String::Split(source->m_content, ',')};

	for (const auto &data : contentSplit)
	{
		std::string name;
		auto value{String::Trim(data)};

		if (String::Contains(value, ":"))
		{
			name = String::Trim(value.substr(0, value.find(':')));
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

			thisValue->SetAttribute(name, value);
		}
		else
		{
			thisValue->AddChild(std::make_unique<Metadata>(name, value));
		}
	}

	for (const auto &child : source->m_children)
	{
		Convert(child.get(), thisValue, false);
	}
}

void Json::AppendData(const Metadata *source, std::ostream *outStream, const int32_t &indentation, const Format &format, const bool &end)
{
	std::stringstream indents;

	if (format != Format::Minified)
	{
		for (int32_t i{}; i < indentation; i++)
		{
			indents << "  ";
		}
	}

	auto openBrace{'{'};
	auto closeBrace{'}'};

	for (const auto &child : source->GetChildren())
	{
		if (child->GetName().empty())
		{
			openBrace = '[';
			closeBrace = ']';
			break;
		}
	}

	*outStream << indents.str();

	if (source->GetName().empty() && source->GetValue().empty())
	{
		*outStream << openBrace;
	}
	else if (source->GetValue().empty())
	{
		*outStream << "\"" << source->GetName();
		*outStream << (format != Format::Minified ? "\": " : "\":");
		*outStream << openBrace;
	}
	else
	{
		if (source->GetName().empty())
		{
			*outStream << String::FixReturnTokens(source->GetValue());
		}
		else
		{
			*outStream << "\"" << source->GetName();
			*outStream << (format != Format::Minified ? "\": " : "\":");
			*outStream << String::FixReturnTokens(source->GetValue());
		}

		if (!(end && source->GetAttributes().empty()))
		{
			*outStream << (format != Format::Minified ? ", " : ",");
		}
	}

	if (format != Format::Minified)
	{
		*outStream << '\n';
	}

	for (const auto &attribute : source->GetAttributes())
	{
		*outStream << indents.str() << "  \"_" << attribute.first << "\": \"" << attribute.second << "\"";

		if (!(end && source->GetChildren().empty()))
		{
			*outStream << (format != Format::Minified ? ", " : ",");
		}

		if (format != Format::Minified)
		{
			*outStream << '\n';
		}
	}

	for (const auto &child : source->GetChildren())
	{
		AppendData(child.get(), outStream, indentation + 1, format, child == source->GetChildren().back());
	}

	if (source->GetValue().empty())
	{
		*outStream << indents.str() << closeBrace;

		if (!(end || indentation == 0))
		{
			*outStream << ',';
		}

		if (format != Format::Minified)
		{
			*outStream << '\n';
		}
	}
}
}
