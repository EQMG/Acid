#include "JsonSection.hpp"

namespace fl
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

	void JsonSection::AppendData(LoadedValue * loadedValue, std::string &data, const int &indentation, const bool &end)
	{
		//	printf("%s = %s\n", loadedValue->GetName().c_str(), loadedValue->GetValue().c_str());
		//	for (auto &child : *loadedValue->GetChildren())
		//	{
		//		AppendData(child, data, indentation + 1, child == loadedValue->GetChildren()->back());
		//	}

		std::string indent;

		for (int i = 0; i < indentation; i++)
		{
			indent += "  ";
		}

		data += indent;

		if (loadedValue->GetName().empty())
		{
			data += "{\n";
		}
		else if (loadedValue->GetValue().empty())
		{
			data += "\"" + loadedValue->GetName() + "\": {\n";
		}
		else
		{
			data += "\"" + loadedValue->GetName() + "\": " + loadedValue->GetValue();

			if (!end)
			{
				data += ", ";
			}

			data += "\n";
		}

		for (auto &child : loadedValue->GetChildren())
		{
			AppendData(child, data, indentation + 1, child == loadedValue->GetChildren().back());
		}

		if (loadedValue->GetName().empty())
		{
			data += indent;
			data += "}\n";
		}
		else if (loadedValue->GetValue().empty())
		{
			data += indent;

			if (end)
			{
				data += "}\n";
			}
			else
			{
				data += "},\n";
			}
		}
	}

	LoadedValue * JsonSection::Convert(const JsonSection &source, LoadedValue * parent, const bool &isTopSection)
	{
		auto thisValue = parent;

		if (!isTopSection)
		{
			thisValue = new LoadedValue(parent, source.m_name, "");
			parent->GetChildren().emplace_back(thisValue);

			auto contentSplit = FormatString::Split(source.m_content, ",", true);

			for (auto &data : contentSplit)
			{
				auto dataSplit = FormatString::Split(data, ":", true);

				if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
				{
					continue;
				}

				auto newChild = new LoadedValue(thisValue, dataSplit.at(0), dataSplit.at(1));
				thisValue->GetChildren().emplace_back(newChild);
			}
		}

		for (auto &child : source.m_children)
		{
			Convert(*child, thisValue, false);
		}

		return thisValue;
	}
}
