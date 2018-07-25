#include "JsonSection.hpp"

namespace acid
{
	JsonSection::JsonSection(std::shared_ptr<JsonSection> parent, const std::string &name, const std::string &content) :
		m_parent(parent),
		m_children(std::vector<std::shared_ptr<JsonSection>>()),
		m_name(name),
		m_content(content)
	{
	}

	JsonSection::~JsonSection()
	{
	}

	void JsonSection::Clear()
	{
		m_parent = nullptr;

		for (auto &child : m_children)
		{
			child->Clear();
		}

		m_children.clear();
	}

	void JsonSection::AppendData(LoadedValue *loadedValue, std::stringstream &builder, const int &indentation, const bool &end)
	{
		std::stringstream indents;

		for (int i = 0; i < indentation; i++)
		{
			indents << "  ";
		}

		builder << indents.str();

		if (loadedValue->GetName().empty())
		{
			builder << "{\n";
		}
		else if (loadedValue->GetValue().empty())
		{
			builder << "\"" << loadedValue->GetName() << "\": {\n";
		}
		else
		{
			builder << "\"" << loadedValue->GetName() + "\": " << loadedValue->GetValue();

			if (!end)
			{
				builder << ", ";
			}

			builder << "\n";
		}

		for (auto &child : loadedValue->GetChildren())
		{
			AppendData(child, builder, indentation + 1, child == loadedValue->GetChildren().back());
		}

		if (loadedValue->GetName().empty())
		{
			builder << indents.str() << "}\n";
		}
		else if (loadedValue->GetValue().empty())
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

	LoadedValue *JsonSection::Convert(const JsonSection &source, LoadedValue *parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		if (!isTopSection)
		{
			thisValue = new LoadedValue(parent, source.m_name, "");
			parent->GetChildren().emplace_back(thisValue);
		}

		auto contentSplit = FormatString::Split(source.m_content, ",", true);

		for (auto &data : contentSplit)
		{
			auto dataSplit = FormatString::Split(data, ":", true);

			if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
			{
				continue;
			}

			std::string name = dataSplit.at(0).substr(1, dataSplit.at(0).size() - 2);
			auto newChild = new LoadedValue(thisValue, name, dataSplit.at(1));
			thisValue->GetChildren().emplace_back(newChild);
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
