#include "LoadedValue.hpp"

namespace fl
{
	LoadedValue::LoadedValue(LoadedValue *parent, const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_parent(parent),
		m_children(std::vector<LoadedValue *>()),
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value),
		m_attributes(attributes)
	{
	}

	LoadedValue::~LoadedValue()
	{
		for (auto &child : m_children)
		{
			delete child;
		}
	}

	std::vector<LoadedValue *> LoadedValue::GetChildren(const std::string &name)
	{
		auto result = std::vector<LoadedValue *>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child);
			}
		}

		return result;
	}

	LoadedValue *LoadedValue::GetChild(const std::string &name, const bool &addIfNull, const bool &reportError)
	{
		std::string nameNoSpaces = FormatString::Replace(name, " ", "_");

		for (auto &child : m_children)
		{
			if (child->m_name == name || child->m_name == nameNoSpaces)
			{
				return child;
			}
		}

		if (!addIfNull)
		{
			if (reportError)
			{
				fprintf(stderr, "Could not find child in loaded value '%s' of name '%s'\n", m_name.c_str(), name.c_str());
			}

			return nullptr;
		}

		auto child = new LoadedValue(this, name, "");
		m_children.emplace_back(child);
		return child;
	}

	LoadedValue *LoadedValue::GetChild(const unsigned int &index, const bool &addIfNull, const bool &reportError)
	{
		if (m_children.size() >= index)
		{
			return m_children.at(index);
		}

		// TODO
		//if (!addIfNull)
		//{
		if (reportError)
		{
			fprintf(stderr, "Could not find child in loaded value '%s' at '%i'\n", m_name.c_str(), index);
		}

		return nullptr;
		//}
	}

	LoadedValue *LoadedValue::GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError)
	{
		auto children = GetChildren(childName);

		if (children.empty())
		{
			return nullptr;
		}

		for (auto &child : children)
		{
			std::string attrib = child->GetAttribute(attribute);

			if (attrib == value)
			{
				return child;
			}
		}

		if (reportError)
		{
			fprintf(stderr, "Could not find child in loaded value '%s' with '%s'\n", m_name.c_str(), attribute.c_str());
		}

		return nullptr;
	}

	void LoadedValue::AddChild(LoadedValue *value)
	{
		auto child = GetChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return;
		}

		child = value;
		m_children.emplace_back(child);
	}

	std::string LoadedValue::GetAttribute(const std::string &attribute) const
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			return nullptr;
		}

		return (*it).second;
	}

	void LoadedValue::AddAttribute(const std::string &attribute, const std::string &value)
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			m_attributes.emplace(attribute, value);
		}

		(*it).second = value;
	}

	bool LoadedValue::RemoveAttribute(const std::string &attribute)
	{
		auto it = m_attributes.find(attribute);

		if (it != m_attributes.end())
		{
			m_attributes.erase(it);
			return true;
		}

		return false;
	}

	std::string LoadedValue::GetString()
	{
		return FormatString::RemoveAll(m_value, '\"');
	}

	void LoadedValue::SetString(const std::string &data)
	{
		m_value = "\"" + data + "\"";
	}

	void LoadedValue::PrintDebug(LoadedValue *value, const bool &content, const int &level)
	{
		std::string tabs = "";

		for (int i = 0; i < level; i++)
		{
			tabs += "  ";
		}

		bool empty = value->GetName().empty() && level != 0;

		if (!empty)
		{
			std::string attributes = "";

			for (auto &attribute : value->m_attributes)
			{
				attributes += attribute.first + "=\"" + attribute.second + "\" ";
			}

			if (content)
			{
				fprintf(stdout, "%s- '%s' (%s): '%s'\n", tabs.c_str(), value->GetName().c_str(), attributes.c_str(), value->GetValue().c_str());
			}
			else
			{
				fprintf(stdout, "%s- '%s' (%s)\n", tabs.c_str(), value->GetName().c_str(), attributes.c_str());
			}
		}

		for (auto &child : value->GetChildren())
		{
			PrintDebug(child, content, empty ? level : level + 1);
		}
	}
}
