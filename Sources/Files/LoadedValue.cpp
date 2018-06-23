#include "LoadedValue.hpp"

namespace fl
{
	LoadedValue::LoadedValue(std::shared_ptr<LoadedValue> parent, const std::string &name, const std::string &value) :
		m_parent(parent),
		m_children(std::vector<std::shared_ptr<LoadedValue>>()),
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value)
	{
	}

	LoadedValue::~LoadedValue()
	{
	}

	std::shared_ptr<LoadedValue> LoadedValue::GetChild(const std::string &name, const bool &addIfNull)
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
			return nullptr;
		}

		auto child = std::make_shared<LoadedValue>(shared_from_this(), name, "");
		m_children.emplace_back(child);
		return child;
	}

	std::shared_ptr<LoadedValue> LoadedValue::GetChild(const unsigned int &index, const bool &addIfNull)
	{
		if (m_children.size() >= index)
		{
			return m_children.at(index);
		}

		// TODO
		//if (!addIfNull)
		//{
		return nullptr;
		//}
	}

	std::shared_ptr<LoadedValue> LoadedValue::GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value)
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

		return nullptr;
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
