#include "JsonSection.hpp"

namespace Flounder
{
	void JsonSection::AppendData(LoadedValue *loadedValue, std::string *data, const int &indentation, const bool &end)
	{
		std::string indent;

		for (int i = 0; i < indentation; i++)
		{
			indent += "  ";
		}

		*data += indent;

		if (loadedValue->m_name.empty())
		{
			*data += "{\n";
		}
		else if (loadedValue->m_value.empty())
		{
			*data += "\"" + loadedValue->m_name + "\": {\n";
		}
		else
		{
			*data += "\"" + loadedValue->m_name + "\": " + loadedValue->m_value;

			if (!end)
			{
				*data += ", ";
			}

			*data += "\n";
		}

		for (auto child : loadedValue->m_children)
		{
			AppendData(child, data, indentation + 1, child == loadedValue->m_children.back());
		}

		if (loadedValue->m_name.empty())
		{
			*data += indent;
			*data += "}\n";
		}
		else if (loadedValue->m_value.empty())
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

	LoadedValue *JsonSection::Convert(JsonSection *source, LoadedValue *destination)
	{
		LoadedValue *thisValue;

		if (destination != nullptr)
		{
			thisValue = new LoadedValue(destination, source->m_name, "");
			destination->m_children.push_back(thisValue);

			auto contentSplit = FormatString::Split(source->m_content, ",");

			for (auto data : contentSplit)
			{
				auto dataSplit = FormatString::Split(data, ":", true);

				if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
				{
					continue;
				}

				LoadedValue *newChild = new LoadedValue(thisValue, dataSplit.at(0), dataSplit.at(1));
				thisValue->m_children.push_back(newChild);
			}
		}
		else
		{
			destination = new LoadedValue(nullptr, "", "");
			thisValue = destination;
		}

		for (auto child : source->m_children)
		{
			Convert(child, thisValue);
		}

		return thisValue;
	}
}
