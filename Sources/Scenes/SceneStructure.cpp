#include "SceneStructure.hpp"

namespace fl
{
	SceneStructure::SceneStructure() :
		ISpatialStructure(),
		m_objects(new std::vector<GameObject *>())
	{
	}

	SceneStructure::~SceneStructure()
	{
		delete m_objects;
	}

	void SceneStructure::Add(GameObject *object)
	{
		m_objects->push_back(object);
	}

	void SceneStructure::Remove(GameObject *object)
	{
		for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
		{
			if (*it == object)
			{
				m_objects->erase(it);
				return;
			}
		}
	}

	void SceneStructure::Clear()
	{
		m_objects->clear();
	}

	std::vector<GameObject *> *SceneStructure::QueryAll(std::vector<GameObject *> *result)
	{
		for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
		{
			result->push_back(*it);
		}

		return result;
	}

	std::vector<GameObject *> *SceneStructure::QueryFrustum(Frustum *range, std::vector<GameObject *> *result)
	{
		for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
		{
			auto gameObject = static_cast<GameObject *>(*it);
			auto rigidbody = gameObject->GetComponent<Rigidbody>();

			if (rigidbody == nullptr || rigidbody->GetCollider()->InFrustum(*range))
			{
				result->push_back(*it);
			}
		}

		return result;
	}

	std::vector<GameObject *> *SceneStructure::QueryBounding(ICollider *range, std::vector<GameObject *> *result)
	{
		for (auto it = m_objects->begin(); it != m_objects->end(); ++it)
		{
			auto gameObject = static_cast<GameObject *>(*it);
			auto rigidbody = gameObject->GetComponent<Rigidbody>();

			if (rigidbody == nullptr || range->Intersects(*rigidbody->GetCollider()).IsIntersection() || range->Contains(*rigidbody->GetCollider()))
			{
				result->push_back(*it);
			}
		}

		return result;
	}

	bool SceneStructure::Contains(GameObject *object)
	{
		return std::find(m_objects->begin(), m_objects->end(), object) != m_objects->end();
	}
}
