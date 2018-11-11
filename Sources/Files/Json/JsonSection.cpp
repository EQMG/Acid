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
			indents << "\t";
		}

		char openBrace = '{';
		char closeBrace = '}';

		for (auto &child : source.GetChildren())
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

			if (!end)
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (auto &child : source.GetChildren())
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

		for (auto &data : contentSplit)
		{
			auto dataSplit = String::Split(data, ":", true);

			if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
			{
				continue;
			}

			std::string name = dataSplit.at(0).substr(1, dataSplit.at(0).size() - 2);
			auto newChild = new Metadata(name, dataSplit.at(1));
			thisValue->AddChild(newChild);
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
