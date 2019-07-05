#include "Resources.hpp"

namespace acid
{
Resources::Resources() :
	m_elapsedPurge{5s}
{
}

void Resources::Update()
{
	if (m_elapsedPurge.GetElapsed() != 0)
	{
		for (auto it{m_resources.begin()}; it != m_resources.end();)
		{
			if ((*it).second.use_count() <= 1)
			{
				it = m_resources.erase(it);
				continue;
			}

			++it;
		}
	}
}

std::shared_ptr<Resource> Resources::Find(const Node &node) const
{
	for (const auto &[key, resource] : m_resources)
	{
		if (key == node)
		{
			return resource;
		}
	}

	return nullptr;
	/*auto it{m_resources.find(node)};

	if (it == m_resources.end())
	{
		return nullptr;
	}

	return it->second;*/
}

void Resources::Add(const Node &node, const std::shared_ptr<Resource> &resource)
{
	if (Find(node) != nullptr)
	{
		return;
	}

	m_resources.emplace(node, resource);
}

void Resources::Remove(const std::shared_ptr<Resource> &resource)
{
	for (auto it{m_resources.begin()}; it != m_resources.end(); ++it) // TODO: Clean remove.
	{
		if ((*it).second == resource)
		{
			m_resources.erase(it);
		}
	}
}
}
