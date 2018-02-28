#include "ValueJson.hpp"

namespace Flounder
{
	ValueJson::ValueJson(ValueJson *parent, const std::string &name, const std::string &value) :
		m_parent(parent),
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value)
	{
	}

	ValueJson::~ValueJson()
	{
		for (auto child : m_children)
		{
			delete child;
		}
	}

	ValueJson *ValueJson::GetChild(const std::string &name)
	{
		for (auto child : m_children)
		{
			if (child->m_name == name)
			{
				return child;
			}
		}

		return nullptr;
	}

	void ValueJson::AppendData(std::string *data, const int &indentation, const bool &end)
	{
		std::string indent;

		for (int i = 0; i < indentation; i++)
		{
			indent += "  ";
		}

		*data += indent;

		if (m_name.empty())
		{
			*data += "{\n";
		}
		else if (m_value.empty())
		{
			*data += "\"" + m_name + "\": {\n";
		}
		else
		{
			*data += "\"" + m_name + "\": " + m_value;

			if (!end)
			{
				*data += ", ";
			}

			*data += "\n";
		}

		for (auto child : m_children)
		{
			child->AppendData(data, indentation + 1, child == m_children.back());
		}

		if (m_name.empty())
		{
			*data += indent;
			*data += "}\n";
		}
		else if (m_value.empty())
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

	ValueJson *ValueJson::Convert(JsonSection *source, ValueJson *destination)
	{
		ValueJson *thisValue;

		if (destination != nullptr)
		{
			thisValue = new ValueJson(destination, source->m_name, "");
			destination->m_children.push_back(thisValue);

			auto contentSplit = FormatString::Split(source->m_content, ",");

			for (auto data : contentSplit)
			{
				auto dataSplit = FormatString::Split(data, ":", true);

				if (dataSplit.size() != 2 || dataSplit.at(0).empty() || dataSplit.at(1).empty())
				{
					continue;
				}

				ValueJson *newChild = new ValueJson(thisValue, dataSplit.at(0), dataSplit.at(1));
				thisValue->m_children.push_back(newChild);
			}
		}
		else
		{
			destination = new ValueJson(nullptr, "", "");
			thisValue = destination;
		}

		for (auto child : source->m_children)
		{
			Convert(child, thisValue);
		}

		return thisValue;
	}
}
