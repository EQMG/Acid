#include "Metadata.hpp"

#include "Engine/Log.hpp"

namespace acid
{
Metadata::Metadata(const std::string &name, const std::string &value, std::map<std::string, std::string> attributes) :
	m_name(String::Trim(String::RemoveAll(name, '\"'))), // TODO: Remove first and last.
	m_value(String::Trim(value)),
	m_attributes(std::move(attributes))
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
	m_children.emplace_back(child);
	return child;
}

void Metadata::RemoveChild(Metadata *child)
{
	m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [&](std::unique_ptr<Metadata> &c)
	{
		return c.get() == child;
	}), m_children.end());
}

std::vector<Metadata *> Metadata::FindChildren(const std::string &name) const
{
	std::vector<Metadata *> result = {};

	for (const auto &child : m_children)
	{
		if (child->m_name == name)
		{
			result.emplace_back(child.get());
		}
	}

	return result;
}

Metadata *Metadata::FindChild(const std::string &name, const bool &reportError) const
{
	std::string nameNoSpaces = String::ReplaceAll(name, " ", "_");

	for (const auto &child : m_children)
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

Metadata *Metadata::FindChildWithBackup(const std::string &name, const std::string &backupName, const bool &reportError) const
{
	auto result = FindChild(name, reportError);

	if (result != nullptr)
	{
		return result;
	}

	return FindChild(backupName, reportError);
}

Metadata *Metadata::FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value, const bool &reportError) const
{
	auto children = FindChildren(childName);

	if (children.empty())
	{
		return nullptr;
	}

	for (const auto &child : children)
	{
		auto attrib = child->FindAttribute(attribute);

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
		return;
	}

	(*it).second = value;
}

void Metadata::RemoveAttribute(const std::string &attribute)
{
	auto it = m_attributes.find(attribute);

	if (it != m_attributes.end()) // TODO: Clean remove.
	{
		m_attributes.erase(it);
	}
}

std::string Metadata::FindAttribute(const std::string &attribute) const
{
	auto it = m_attributes.find(attribute);

	if (it == m_attributes.end())
	{
		return "";
	}

	return (*it).second;
}

Metadata *Metadata::Clone() const
{
	auto result = new Metadata(m_name, m_value, m_attributes);

	for (const auto &child : m_children)
	{
		result->m_children.emplace_back(child->Clone());
	}

	return result;
}

bool Metadata::operator==(const Metadata &other) const
{
	return m_name == other.m_name && m_value == other.m_value && m_attributes == other.m_attributes && m_children.size() == other.m_children.size()
		&& std::equal(m_children.begin(), m_children.end(), other.m_children.begin(), [](const std::unique_ptr<Metadata> &left, const std::unique_ptr<Metadata> &right)
		{
			return *left == *right;
		});
}

bool Metadata::operator!=(const Metadata &other) const
{
	return !(*this == other);
}

bool Metadata::operator<(const Metadata &other) const
{
	return m_name < other.m_name || m_value < other.m_value || m_attributes < other.m_attributes || m_children < other.m_children;
}

void Metadata::Load(std::istream *inStream)
{
}

void Metadata::Write(std::ostream *outStream) const
{
}
}
