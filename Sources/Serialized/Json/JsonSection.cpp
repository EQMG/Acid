#include "JsonSection.hpp"

#include "Helpers/String.hpp"

namespace acid
{
	JsonSection::JsonSection(JsonSection *parent, const std::string &name, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<std::unique_ptr<JsonSection>>()),
		m_name(name),
		m_content(content)
	{
	}

	void JsonSection::AppendData(const Metadata &source, std::stringstream &builder, const int32_t &indentation, const bool &end)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		char openBrace = '{';
		char closeBrace = '}';

		for (const auto &child : source.GetChildren())
		{
			if (child->GetName().empty())
			{
				openBrace = '[';
				closeBrace = ']';
				break;
			}
		}

		builder << indents.str();

		if (source.GetName().empty())
		{
			builder << openBrace << "\n";
		}
		else if (source.GetValue().empty())
		{
			builder << "\"" << source.GetName() << "\": " << openBrace << "\n";
		}
		else
		{
			builder << "\"" << source.GetName() + "\": " << source.GetValue();

			if (!(end && source.GetAttributes().empty()))
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (const auto &attribute : source.GetAttributes())
		{
			builder << indents.str() << "  \"_" << attribute.first + "\": \"" << attribute.second << "\"";

			if (!(end && source.GetChildren().empty()))
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (const auto &child : source.GetChildren())
		{
			AppendData(*child, builder, indentation + 1, child == source.GetChildren().back());
		}

		if (source.GetValue().empty())
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

	Metadata *JsonSection::Convert(const JsonSection &source, Metadata *parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		if (!isTopSection)
		{
			thisValue = new Metadata(source.m_name, "");
			parent->AddChild(thisValue);
		}

		auto contentSplit = String::Split(source.m_content, ",", true);

		for (const auto &data : contentSplit)
		{
			auto name = String::Trim(data.substr(0, data.find(':')));
			auto value = String::ReplaceFirst(data, name, "");
			value = String::Trim(value.erase(0, 1));

			if (name.empty() || value.empty())
			{
				continue;
			}

			name = name.substr(1, name.size() - 2);

			if (String::StartsWith(name, "_"))
			{
				name = name.erase(0, 1);
				value = value.substr(1, value.size() - 2);
				thisValue->AddAttribute(name, value);
			}
			else
			{
				auto newChild = new Metadata(name, value);
				thisValue->AddChild(newChild);
			}
		}

		for (const auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
