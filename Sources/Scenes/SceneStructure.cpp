#include "SceneStructure.hpp"

#include "Physics/Collider.hpp"

namespace acid
{
	SceneStructure::SceneStructure() :
		ISpatialStructure(),
		m_objects(std::vector<std::shared_ptr<GameObject>>())
	{
	}

	SceneStructure::~SceneStructure()
	{
	}

	void SceneStructure::Add(const std::shared_ptr<GameObject> &object)
	{
		m_objects.emplace_back(object);
	}

	bool SceneStructure::Remove(const std::shared_ptr<GameObject> &object)
	{
		auto it = std::find(m_objects.begin(), m_objects.end(), object);

		if (it != m_objects.end())
		{
			m_objects.erase(it);
			return true;
		}

		return false;
	}

	void SceneStructure::Clear()
	{
		m_objects.clear();
	}

	std::vector<std::shared_ptr<GameObject>> SceneStructure::QueryAll()
	{
		auto result = std::vector<std::shared_ptr<GameObject>>();

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			result.emplace_back(*it);
		}

		return result;
	}

	std::vector<std::shared_ptr<GameObject>> SceneStructure::QueryFrustum(const Frustum &range)
	{
		auto result = std::vector<std::shared_ptr<GameObject>>();

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			auto gameObject = std::dynamic_pointer_cast<GameObject>(*it);
			auto collider = gameObject->GetComponent<Collider>();

			if (collider == nullptr || collider->InFrustum(range))
			{
				result.emplace_back(*it);
			}
		}

		return result;
	}

	/*std::vector<std::shared_ptr<GameObject>> SceneStructure::QueryBounding(Collider *range)
	{
		auto result = std::vector<std::shared_ptr<GameObject>>();

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			auto gameObject = std::dynamic_pointer_cast<GameObject>(*it);
			auto collider = gameObject->GetComponent<Collider>();

			if (collider == nullptr || range->Intersects(*collider).IsIntersection() || range->Contains(*collider))
			{
				result.emplace_back(*it);
			}
		}

		return result;
	}*/

	bool SceneStructure::Contains(const std::shared_ptr<GameObject> &object)
	{
		return std::find(m_objects.begin(), m_objects.end(), object) != m_objects.end();
	}
}
