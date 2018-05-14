#include "JsonSection.hpp"

namespace fl
{
	void JsonSection::AppendData(LoadedValue *loadedValue, std::string *data, const int &indentation, const bool &end)
	{
		std::string indent;

		for (int i = 0; i < indentation; i++)
		{
			indent += "  ";
		}

		*data += indent;

		if (loadedValue->GetName().empty())
		{
			*data += "{\n";
		}
		else if (loadedValue->GetValue().empty())
		{
			*data += "\"" + loadedValue->GetName() + "\": {\n";
		}
		else
		{
			*data += "\"" + loadedValue->GetName() + "\": " + loadedValue->GetValue();

			if (!end)
			{
				*data += ", ";
			}

			*data += "\n";
		}

		for (auto child : *loadedValue->GetChildren())
		{
			AppendData(child, data, indentation + 1, child == loadedValue->GetChildren()->back());
		}

		if (loadedValue->GetName().empty())
		{
			*data += indent;
			*data += "}\n";
		}
		else if (loadedValue->GetValue().empty())
		{
			*data += indent;

			if (end)
			{
				*data += "}\n";
			}
			else
			{
				*data += "},\n";
			}
		}
	}

	LoadedValue *JsonSection::Convert(JsonSection *source, LoadedValue *parent, const bool &isTopSection)
	{
		LoadedValue *thisValue = parent;

		if (!isTopSection)
		{
			thisValue = new LoadedValue(parent, source->m_name, "");
			parent->GetChildren()->push_back(thisValue);

			auto contentSplit = FormatString::Split(source->m_content, ",");

			for (auto data : contentSplit)
			{
				auto dataSplit = FormatString::Split(data, ":", true);

				if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
				{
					continue;
				}

				LoadedValue *newChild = new LoadedValue(thisValue, dataSplit.at(0), dataSplit.at(1));
				thisValue->GetChildren()->push_back(newChild);
			}
		}

		for (auto child : source->m_children)
		{
			Convert(child, thisValue, false);
		}

		return thisValue;
	}
}
