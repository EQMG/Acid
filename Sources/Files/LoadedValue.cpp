#include "LoadedValue.hpp"

#include <algorithm>
#include <ostream>

namespace acid
{
	LoadedValue::LoadedValue(const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_children(std::vector<std::shared_ptr<LoadedValue>>()),
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value),
		m_attributes(attributes)
	{
	}

	LoadedValue::LoadedValue(const LoadedValue &source) :
		m_children(source.m_children),
		m_name(source.m_name),
		m_value(source.m_value),
		m_attributes(source.m_attributes)
	{
	}

	LoadedValue::~LoadedValue()
	{
	}

	std::vector<std::shared_ptr<LoadedValue>> LoadedValue::FindChildren(const std::string &name) const
	{
		auto result = std::vector<std::shared_ptr<LoadedValue>>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child);
			}
		}

		return result;
	}

	std::shared_ptr<LoadedValue> LoadedValue::FindChild(const std::string &name, const bool &addIfNull,
	                                                    const bool &reportError)
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

		auto child = std::make_shared<LoadedValue>(name, "");
		m_children.emplace_back(child);
		return child;
	}

	std::shared_ptr<LoadedValue> LoadedValue::FindChild(const uint32_t &index, const bool &reportError) const
	{
		if (m_children.size() >= index)
		{
			return m_children.at(index);
		}

		if (reportError)
		{
			fprintf(stderr, "Could not find child in loaded value '%s' at '%i'\n", m_name.c_str(), index);
		}

		return nullptr;
	}

	std::shared_ptr<LoadedValue> LoadedValue::FindChildWithAttribute(const std::string &childName,
	                                                                 const std::string &attribute,
	                                                                 const std::string &value, const bool &reportError) const
	{
		auto children = FindChildren(childName);

		if (children.empty())
		{
			return nullptr;
		}

		for (auto &child : children)
		{
			std::string attrib = child->FindAttribute(attribute);

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

	void LoadedValue::AddChild(const std::shared_ptr<LoadedValue> &value)
	{
		auto child = FindChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return;
		}

		m_children.emplace_back(value);
	}

	bool LoadedValue::RemoveChild(const std::shared_ptr<LoadedValue> &value)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), value), m_children.end());
		return true;
	}

	std::string LoadedValue::FindAttribute(const std::string &attribute) const
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
}
