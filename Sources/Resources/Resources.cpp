#include "Resources.hpp"

namespace fl
{
	Resources::Resources() :
		m_resources(std::vector<std::shared_ptr<IResource>>()),
		m_timerPurge(Timer(3.0f))
	{
	}

	Resources::~Resources()
	{
	}

	void Resources::Update()
	{
		if (m_timerPurge.IsPassedTime())
		{
			m_timerPurge.ResetStartTime();

			for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
			{
				if ((*it).use_count() <= 1)
				{
					printf("Resource '%s' erased\n", (*it)->GetFilename().c_str());
					m_resources.erase(it);
				}
			}
		}
	}

	std::shared_ptr<IResource> Resources::Get(const std::string &filename)
	{
		for (auto resource : m_resources)
		{
			if (resource != nullptr && resource->GetFilename() == filename)
			{
				return resource;
			}
		}

		return nullptr;
	}

	void Resources::Add(std::shared_ptr<IResource> resource)
	{
		m_resources.emplace_back(resource);
	}

	void Resources::Remove(std::shared_ptr<IResource> resource)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if (*it == resource)
			{
				m_resources.erase(it);
			}
		}
	}

	void Resources::Remove(const std::string &filename)
	{
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if ((*it)->GetFilename() == filename)
			{
				m_resources.erase(it);
			}
		}
	}
}
