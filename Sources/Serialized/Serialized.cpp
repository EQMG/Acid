#include "Serialized.hpp"

#include <algorithm>
#include <ostream>

namespace acid
{
	Serialized::Serialized(const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value),
		m_children(std::vector<std::shared_ptr<Serialized>>()),
		m_attributes(attributes)
	{
	}

	Serialized::Serialized(const Serialized &source) :
		m_name(source.m_name),
		m_value(source.m_value),
		m_children(source.m_children),
		m_attributes(source.m_attributes)
	{
	}

	Serialized::~Serialized()
	{
	}

	std::string Serialized::GetString() const
	{
		return FormatString::RemoveAll(m_value, '\"');
	}

	void Serialized::SetString(const std::string &data)
	{
		m_value = "\"" + data + "\"";
	}

	std::shared_ptr<Serialized> Serialized::AddChild(const std::shared_ptr<Serialized> &value)
	{
		auto child = FindChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return child;
		}

		m_children.emplace_back(value);
		return value;
	}

	bool Serialized::RemoveChild(const std::shared_ptr<Serialized> &value)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), value), m_children.end());
		return true;
	}

	std::vector<std::shared_ptr<Serialized>> Serialized::FindChildren(const std::string &name) const
	{
		auto result = std::vector<std::shared_ptr<Serialized>>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child);
			}
		}

		return result;
	}

	std::shared_ptr<Serialized> Serialized::FindChild(const std::string &name) const
	{
		std::string nameNoSpaces = FormatString::Replace(name, " ", "_");

		for (auto &child : m_children)
		{
			if (child->m_name == name || child->m_name == nameNoSpaces)
			{
				return child;
			}
		}

		return {};
	}

	std::shared_ptr<Serialized> Serialized::FindChildSafe(const std::string &name)
	{
		auto child = FindChild(name);

		if (child != nullptr)
		{
			return child;
		}

		child = std::make_shared<Serialized>(name, "");
		m_children.emplace_back(child);
		return child;
	}

	std::shared_ptr<Serialized> Serialized::FindChildWithAttribute(const std::string &childName, const std::string &attribute,
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

	void Serialized::AddAttribute(const std::string &attribute, const std::string &value)
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			m_attributes.emplace(attribute, value);
		}

		(*it).second = value;
	}

	bool Serialized::RemoveAttribute(const std::string &attribute)
	{
		auto it = m_attributes.find(attribute);

		if (it != m_attributes.end())
		{
			m_attributes.erase(it);
			return true;
		}

		return false;
	}

	std::string Serialized::FindAttribute(const std::string &attribute) const
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			return nullptr;
		}

		return (*it).second;
	}
}
