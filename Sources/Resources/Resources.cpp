#include "Resources.hpp"

#include <algorithm>

namespace acid
{
	Resources::Resources() :
		m_mutex(std::mutex()),
		m_resources(std::map<Metadata, std::shared_ptr<Resource>>()),
		m_timerPurge(Timer(Time::Seconds(5.0f)))
	{
	}

	void Resources::Update()
	{
		if (m_timerPurge.IsPassedTime())
		{
			m_timerPurge.ResetStartTime();

			std::lock_guard<std::mutex> lock(m_mutex);

			for (auto it = m_resources.begin(); it != m_resources.end();)
			{
				if ((*it).second.use_count() <= 1)
				{
#if defined(ACID_VERBOSE)
				//	Log::Out("Resource '%s' erased\n", (*it)->GetName().c_str());
#endif
					it = m_resources.erase(it);
					continue;
				}

				++it;
			}
		}
	}

	std::shared_ptr<Resource> Resources::Find(const Metadata &metadata) const
	{
		auto it = m_resources.find(metadata);

		if (it == m_resources.end())
		{
			return nullptr;
		}

		return it->second;
	}

	void Resources::Add(const Metadata &metadata, const std::shared_ptr<Resource> &resource)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_resources.find(metadata) != m_resources.end())
		{
			return;
		}

	//	m_resources.emplace(metadata, resource);
	}

	void Resources::Remove(const std::shared_ptr<Resource> &resource)
	{
		/*std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if (*it == resource)
			{
				m_resources.erase(it);
			}
		}*/
	}

	void Resources::Remove(const std::string &filename)
	{
		/*std::lock_guard<std::mutex> lock(m_mutex);

		for (auto it = m_resources.begin(); it != m_resources.end(); ++it)
		{
			if ((*it)->GetName() == filename)
			{
				m_resources.erase(it);
			}
		}*/
	}
}
