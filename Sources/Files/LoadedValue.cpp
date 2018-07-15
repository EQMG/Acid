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

	LoadedValue *LoadedValue::GetChild(const std::string &name, const bool &addIfNull, const bool &reportError)
	{
		for (auto &child : m_children)
		{
			if (child->m_name == name)
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
		if (GetChild(childName) == nullptr)
		{
			return nullptr;
		}

		for (auto &child : GetChild(childName)->m_children)
		{
			auto attrib = child->GetChild(attribute);

			if (attrib != nullptr && attrib->GetString() == value)
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

	/*std::optional<LoadedValue *> LoadedValue::SearchHierarchy(const std::vector<std::string> &names, const int &i)
	{
		if (i == names.size())
		{
			return this;
		}

		auto child = GetChild(names[i]);

		if (!child.has_value())
		{
			return {};
		}

		return child.value()->SearchHierarchy(names, i + 1);
	}

	std::optional<LoadedValue *> LoadedValue::SearchHierarchyAttribs(const std::vector<std::string> &names, const std::string &childName, const std::string &attribute, const std::string &value, const int &i)
	{
		auto child = SearchHierarchy(names, i);

		if (!child.has_value())
		{
			return {};
		}

		return child.value()->GetChildWithAttribute(childName, attribute, value);
	}*/

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
		auto result = m_attributes.find(attribute);

		if (result == m_attributes.end())
		{
			return nullptr;
		}

		return (*result).second;
	}

	void LoadedValue::AddAttribute(const std::string &attribute, const std::string &value)
	{
		auto find = m_attributes.find(attribute);

		if (find == m_attributes.end())
		{
			m_attributes.emplace(attribute, value);
		}

		(*find).second = value;
	}

	bool LoadedValue::RemoveAttribute(const std::string &attribute)
	{
		auto find = m_attributes.find(attribute);

		if (find != m_attributes.end())
		{
			m_attributes.erase(find);
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

	void LoadedValue::PrintDebug(LoadedValue *value, const int &level)
	{
		std::string tabs = "";

		for (int i = 0; i < level; i++)
		{
			tabs += "  ";
		}

		bool empty = value->GetName().empty();

		if (!empty)
		{
			fprintf(stdout, "%s- '%s': '%s'\n", tabs.c_str(), value->GetName().c_str(), value->GetValue().c_str());
		}

		for (auto &child : value->GetChildren())
		{
			PrintDebug(child, empty ? level : level + 1);
		}
	}
}
