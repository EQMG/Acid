#include "Metadata.hpp"

#include <algorithm>
#include "Engine/Log.hpp"

namespace acid
{
	Metadata::Metadata(const std::string &name, const std::string &value, const std::map<std::string, std::string> &attributes) :
		m_name(String::Trim(String::RemoveAll(name, '\"'))),
		m_value(String::Trim(value)),
		m_children(std::vector<std::unique_ptr<Metadata>>()),
		m_attributes(attributes)
	{
	}

	Metadata::Metadata(const std::string &name, const std::string &value) :
		m_name(String::Trim(String::RemoveAll(name, '\"'))),
		m_value(String::Trim(value)),
		m_children(std::vector<std::unique_ptr<Metadata>>()),
		m_attributes(std::map<std::string, std::string>())
	{
	}

	std::string Metadata::GetString() const
	{
		std::string string = m_value;

		if (string.empty())
		{
			return string;
		}

		if (string.front() == '\"')
		{
			string.erase(0, 1);
		}

		if (string.back() == '\"')
		{
			string.pop_back();
		}

		return string;
	}

	void Metadata::SetString(const std::string &data)
	{
		m_value = "\"" + data + "\"";
	}

	Metadata *Metadata::AddChild(Metadata *child)
	{
		/*auto child = FindChild(value->m_name);

		if (child != nullptr)
		{
			child->m_value = value->m_value;
			return child;
		}*/

		m_children.emplace_back(child);
		return child;
	}

	bool Metadata::RemoveChild(Metadata *child)
	{
		for (auto it = m_children.begin(); it != m_children.end(); ++it)
		{
			if ((*it).get() == child)
			{
				m_children.erase(it);
				return true;
			}
		}

		return false;
	}

	std::vector<Metadata *> Metadata::FindChildren(const std::string &name) const
	{
		auto result = std::vector<Metadata *>();

		for (auto &child : m_children)
		{
			if (child->m_name == name)
			{
				result.push_back(child.get());
			}
		}

		return result;
	}

	Metadata *Metadata::FindChild(const std::string &name, const bool &reportError) const
	{
		std::string nameNoSpaces = String::ReplaceAll(name, " ", "_");

		for (auto &child : m_children)
		{
			if (child->m_name == name || child->m_name == nameNoSpaces)
			{
				return child.get();
			}
		}

		if (reportError)
		{
			Log::Error("Could not find child in metadata by name '%s'\n", name.c_str());
		}

		return nullptr;
	}

	Metadata *Metadata::FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError) const
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
			Log::Error("Could not find child in metadata '%s' with '%s'\n", childName.c_str(), attribute.c_str());
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
