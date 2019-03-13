#include "Yaml.hpp"

#include "Files/Files.hpp"
#include "Helpers/String.hpp"

namespace acid
{
Yaml::Yaml() : Metadata("", "") {}

Yaml::Yaml(Metadata* metadata) : Metadata("", "")
{
	AddChildren(metadata, this);
}

void Yaml::Load(std::istream* inStream)
{
	ClearChildren();
	ClearAttributes();

	auto topSection = std::make_unique<Section>(nullptr, "", 0, false);
	Section* currentSection = topSection.get();
	uint32_t lastIndentation = 0;

	size_t lineNum = 0;
	std::string linebuf;

	while(inStream->peek() != -1)
		{
			Files::SafeGetLine(*inStream, linebuf);
			lineNum++;

			// Start marker.
			if(linebuf == "---")
				{
					continue;
				}

			uint32_t indentation = 0;
			uint32_t arrayLevels = 0;
			bool comment = false;

			for(auto it = linebuf.begin(); it != linebuf.end(); ++it)
				{
					if(*it == ' ')
						{
							indentation++;
						}
					else if(*it == '#')
						{
							comment = true;
							break;
						}
					else if(*it == '-' && *(it + 1) == ' ')
						{
							arrayLevels++;
							indentation++;
						}
					else
						{
							break;
						}
				}

			if(comment)
				{
					continue;
				}

			if(indentation < lastIndentation)
				{
					for(uint32_t i = 0; i < (lastIndentation - indentation) / 2; i++)
						{
							if(currentSection->m_parent != nullptr)
								{
									currentSection = currentSection->m_parent;
								}
						}
				}
			else if(indentation > lastIndentation)
				{
					for(uint32_t i = 0; i < ((indentation - lastIndentation) / 2) - 1; i++)
						{
							auto section = new Section(currentSection, "", lastIndentation + (i * 2), arrayLevels);
							currentSection->m_children.emplace_back(section);
							currentSection = section;
						}
				}

			if(indentation - lastIndentation == 2)
				{
					currentSection = currentSection->m_children.back().get();
				}

			auto content = String::Trim(linebuf).erase(0, 2 * arrayLevels);
			auto section = new Section(currentSection, content, indentation, arrayLevels);
			currentSection->m_children.emplace_back(section);
			lastIndentation = indentation;
		}

	Convert(topSection.get(), this, true);
}

void Yaml::Write(std::ostream* outStream) const
{
	*outStream << "---\n";
	AppendData(this, nullptr, outStream, 0);
}

void Yaml::AddChildren(const Metadata* source, Metadata* destination)
{
	for(const auto& child : source->GetChildren())
		{
			auto created = destination->AddChild(new Metadata(child->GetName(), child->GetValue()));
			AddChildren(child.get(), created);
		}

	for(const auto& attribute : source->GetAttributes())
		{
			destination->AddAttribute(attribute.first, attribute.second);
		}
}

void Yaml::Convert(const Section* source, Metadata* parent, const bool& isTopSection)
{
	auto name = String::Trim(source->m_content.substr(0, source->m_content.find(':')));
	auto value = String::Trim(String::ReplaceFirst(source->m_content, name, ""));
	value = String::Trim(value.erase(0, 1));
	bool singleArray = false;

	if(source->m_arrayLevels != 0 && value.empty())
		{
			value = name;
			name = "";
			singleArray = true;
		}

	auto thisValue = parent;

	if(String::StartsWith(name, "_"))
		{
			name = name.erase(0, 1);
			parent->AddAttribute(name, value);
			return;
		}

	if(!isTopSection)
		{
			thisValue = new Metadata(name, value);
			parent->AddChild(thisValue);
		}

	auto tmpValue = thisValue;

	for(const auto& child : source->m_children)
		{
			if(child->m_arrayLevels != 0) //  && !singleArray
				{
					tmpValue = new Metadata();
					thisValue->AddChild(tmpValue);
				}

			Convert(child.get(), tmpValue, false);
		}
}

void Yaml::AppendData(const Metadata* source, const Metadata* parent, std::ostream* outStream, const int32_t& indentation)
{
	std::stringstream indents;

	for(int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

	bool wroteIntents = false;

	if(parent != nullptr && !(parent->GetChildren()[0].get() == source && parent->GetName().empty() && parent->GetValue().empty()))
		{
			*outStream << indents.str();
			wroteIntents = true;
		}

	if(parent != nullptr && parent->GetValue().empty() && parent->GetChildren()[0]->GetName().empty())
		{
			if(wroteIntents)
				{
					outStream->seekp(-2, std::stringstream::cur);
				}

			*outStream << "- ";
		}

	if(!source->GetName().empty())
		{
			if(source->GetValue().empty())
				{
					*outStream << source->GetName() << ": \n";
				}
			else
				{
					*outStream << source->GetName() << ": " << String::FixReturnTokens(source->GetValue()) << '\n';
				}
		}
	else if(!source->GetValue().empty())
		{
			*outStream << String::FixReturnTokens(source->GetValue()) << '\n';
		}

	for(const auto& attribute : source->GetAttributes())
		{
			*outStream << indents.str() << "  _" << attribute.first + ": " << attribute.second << '\n';
		}

	for(const auto& child : source->GetChildren())
		{
			AppendData(child.get(), source, outStream, indentation + !source->GetName().empty());
		}
}
}
