#include "LoadedValue.hpp"

namespace Flounder
{
	LoadedValue::LoadedValue(LoadedValue *parent, const std::string &name, const std::string &value) :
		m_parent(parent),
		m_children(new std::vector<LoadedValue *>()),
		m_name(FormatString::RemoveAll(name, '\"')),
		m_value(value)
	{
	}

	LoadedValue::~LoadedValue()
	{
		for (auto child : *m_children)
		{
			delete child;
		}

		delete m_children;
	}

	LoadedValue *LoadedValue::GetChild(const std::string &name, const bool &addIfNull)
	{
		for (auto child : *m_children)
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

		LoadedValue *child = new LoadedValue(this, m_name, "");
		m_children->push_back(child);
		return child;
	}

	LoadedValue *LoadedValue::GetChild(const unsigned int &index, const bool &addIfNull)
	{
		if (m_children->size() >= index)
		{
			return m_children->at(index);
		}

		// TODO
		//if (!addIfNull)
		//{
		return nullptr;
		//}
	}

	LoadedValue *LoadedValue::GetChildWithAttribute(const std::string &childName, const std::string &attribute, const std::string &value)
	{
		if (GetChild(childName) == nullptr)
		{
			return nullptr;
		}

		for (auto child : *GetChild(childName)->m_children)
		{
			auto attrib = child->GetChild(attribute);

			if (attrib != nullptr && attrib->GetString() == value)
			{
				return child;
			}
		}

		return nullptr;
	}
}
