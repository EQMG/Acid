#include "Metadata.hpp"

#include <algorithm>
#include "Engine/Log.hpp"

namespace acid
{
	Metadata::Metadata(const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_name(String::Trim(String::RemoveAll(name, '\"'))),
		m_value(String::Trim(value)),
		m_children(std::vector<std::shared_ptr<Metadata>>()),
		m_attributes(attributes)
	{
	}

	Metadata::Metadata(const std::string &name, const std::string &value) :
		m_name(String::Trim(String::RemoveAll(name, '\"'))),
		m_value(String::Trim(value)),
		m_children(std::vector<std::shared_ptr<Metadata>>()),
		m_attributes(std::map<std::string, std::string>())
	{
	}

	Metadata::Metadata(const Metadata &source) :
		m_name(source.m_name),
		m_value(source.m_value),
		m_children(source.m_children),
		m_attributes(source.m_attributes)
	{
	}

	Metadata::~Metadata()
	{
	}

	std::string Metadata::GetString() const
	{
		return String::RemoveAll(m_value, '\"'); // FIXME: Just first and last.
	}

	void Metadata::SetString(const std::string &data)
	{
		m_value = "\"" + data + "\"";
	}

	std::shared_ptr<Metadata> Metadata::AddChild(const std::shared_ptr<Metadata> &value)
	{
		/*auto child = FindChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return child;
		}*/

		m_children.emplace_back(value);
		return value;
	}

	bool Metadata::RemoveChild(const std::shared_ptr<Metadata> &value)
	{
		m_children.erase(std::remove(m_children.begin(), m_children.end(), value), m_children.end());
		return true;
	}

	std::vector<std::shared_ptr<Metadata>> Metadata::FindChildren(const std::string &name) const
	{
		auto result = std::vector<std::shared_ptr<Metadata>>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child);
			}
		}

		return result;
	}

	std::shared_ptr<Metadata> Metadata::FindChild(const std::string &name, const bool &reportError) const
	{
		std::string nameNoSpaces = String::Replace(name, " ", "_");

		for (auto &child : m_children)
		{
			if (child->m_name == name || child->m_name == nameNoSpaces)
			{
				return child;
			}
		}

		if (reportError)
		{
			Log::Error("Could not find child in metadata by name '%s'\n", m_name.c_str());
		}

		return nullptr;
	}

	std::shared_ptr<Metadata> Metadata::FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError) const
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
			Log::Error("Could not find child in metadata '%s' with '%s'\n", m_name.c_str(), attribute.c_str());
		}

		return nullptr;
	}

	void Metadata::AddAttribute(const std::string &attribute, const std::string &value)
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			m_attributes.emplace(attribute, value);
		}

		(*it).second = value;
	}

	bool Metadata::RemoveAttribute(const std::string &attribute)
	{
		auto it = m_attributes.find(attribute);

		if (it != m_attributes.end())
		{
			m_attributes.erase(it);
			return true;
		}

		return false;
	}

	std::string Metadata::FindAttribute(const std::string &attribute) const
	{
		auto it = m_attributes.find(attribute);

		if (it == m_attributes.end())
		{
			return nullptr;
		}

		return (*it).second;
	}
}
