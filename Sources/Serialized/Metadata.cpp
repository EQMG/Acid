#include "Metadata.hpp"

#include "Engine/Log.hpp"

namespace acid
{
Metadata::Metadata(const std::string &name, const std::string &value) :
	m_name{String::Trim(String::RemoveAll(name, '\"'))}, // TODO: Remove first and last.
	m_value{String::Trim(value)}
{
}

void Metadata::Load(std::istream *inStream)
{
}

void Metadata::Write(std::ostream *outStream, const Format &format) const
{
}

std::unique_ptr<Metadata> Metadata::Clone() const
{
	auto clone{std::make_unique<Metadata>(m_name, m_value)};
	clone->m_attributes = m_attributes;

	for (const auto &child : m_children)
	{
		clone->m_children.emplace_back(child->Clone());
	}

	return clone;
}

std::string Metadata::GetString() const
{
	auto string{m_value};

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

std::vector<Metadata *> Metadata::FindChildren(const std::string &name) const
{
	std::vector<Metadata *> children;

	for (const auto &child : m_children)
	{
		if (child->m_name == name)
		{
			children.emplace_back(child.get());
		}
	}

	return children;
}

Metadata *Metadata::FindChild(const std::string &name) const
{
	for (const auto &child : m_children)
	{
		if (child->m_name == name)
		{
			return child.get();
		}
	}

	return nullptr;
}

Metadata *Metadata::FindChildWithBackup(const std::string &name, const std::string &backupName) const
{
	auto child{FindChild(name)};

	if (child != nullptr)
	{
		return child;
	}

	return FindChild(backupName);
}

Metadata *Metadata::FindChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value) const
{
	auto children{FindChildren(childName)};

	if (children.empty())
	{
		return nullptr;
	}

	for (const auto &child : children)
	{
		auto attrib{child->FindAttribute(attribute)};

		if (attrib == value)
		{
			return child;
		}
	}

	return nullptr;
}

Metadata *Metadata::AddChild(std::unique_ptr<Metadata> &&child)
{
	return m_children.emplace_back(std::move(child)).get();
}

void Metadata::RemoveChild(Metadata *child)
{
	m_children.erase(std::remove_if(m_children.begin(), m_children.end(), [child](std::unique_ptr<Metadata> &c)
	{
		return c.get() == child;
	}), m_children.end());
}

std::optional<std::string> Metadata::FindAttribute(const std::string &attribute) const
{
	auto it{m_attributes.find(attribute)};

	if (it == m_attributes.end())
	{
		return std::nullopt;
	}

	return (*it).second;
}

void Metadata::SetAttribute(const std::string &attribute, const std::string &value)
{
	auto it{m_attributes.find(attribute)};

	if (it == m_attributes.end())
	{
		m_attributes.emplace(attribute, value);
		return;
	}

	(*it).second = value;
}

void Metadata::RemoveAttribute(const std::string &attribute)
{
	// TODO C++20: Clean remove.
	auto it{m_attributes.find(attribute)};

	if (it != m_attributes.end()) 
	{
		m_attributes.erase(it);
	}
}

Metadata *Metadata::operator[](const std::string &string) const
{
	return FindChild(string);
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
}
