#include "JsonSection.hpp"

#include "Helpers/String.hpp"

namespace acid
{
	JsonSection::JsonSection(JsonSection *parent, const std::string &name, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<JsonSection *>()),
		m_name(name),
		m_content(content)
	{
	}

	JsonSection::~JsonSection()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
	}

	void JsonSection::AppendData(const std::shared_ptr<Metadata> &source, std::stringstream &builder, const int32_t &indentation, const bool &end)
	{
		std::stringstream indents;

		for (int32_t i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		builder << indents.str();

		if (source->GetName().empty())
		{
			builder << "{\n";
		}
		else if (source->GetValue().empty())
		{
			builder << "\"" << source->GetName() << "\": {\n";
		}
		else
		{
			builder << "\"" << source->GetName() + "\": " << source->GetValue();

			if (!end)
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (auto &child : source->GetChildren())
		{
			AppendData(child, builder, indentation + 1, child == source->GetChildren().back());
		}

		if (source->GetName().empty())
		{
			builder << indents.str() << "}\n";
		}
		else if (source->GetValue().empty())
		{
			builder << indents.str();

			if (end)
			{
				builder << "}\n";
			}
			else
			{
				builder << "},\n";
			}
		}
	}

	std::shared_ptr<Metadata> JsonSection::Convert(const JsonSection &source, std::shared_ptr<Metadata> &parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		if (!isTopSection)
		{
			thisValue = std::make_shared<Metadata>(source.m_name, "");
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
			auto newChild = std::make_shared<Metadata>(name, dataSplit.at(1));
			thisValue->AddChild(newChild);
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
