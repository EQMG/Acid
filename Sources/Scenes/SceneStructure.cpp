#include "SceneStructure.hpp"

#include "Physics/Rigidbody.hpp"

namespace acid
{
	SceneStructure::SceneStructure() :
		ISpatialStructure(),
		m_objects(std::vector<std::unique_ptr<GameObject>>())
	{
	}

	void SceneStructure::Add(GameObject *object)
	{
		m_objects.emplace_back(object);
	}

	bool SceneStructure::Remove(GameObject *object)
	{
		for (auto it = --m_objects.end(); it != m_objects.begin(); --it)
		{
			if ((*it).get() != object)
			{
				continue;
			}

			m_objects.erase(it);
			return true;
		}

		return false;
	}

	void SceneStructure::Clear()
	{
		m_objects.clear();
	}

	std::vector<GameObject *> SceneStructure::QueryAll()
	{
		auto result = std::vector<GameObject *>();

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			result.emplace_back((*it).get());
		}

		return result;
	}

	std::vector<GameObject *> SceneStructure::QueryFrustum(const Frustum &range)
	{
		auto result = std::vector<GameObject *>();

		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			auto rigidbody = (*it)->GetComponent<Rigidbody>();

			if (rigidbody == nullptr || rigidbody->InFrustum(range))
			{
				result.emplace_back((*it).get());
			}
		}

		return result;
	}

	/*std::vector<GameObject *> SceneStructure::QuerySphere(const Vector3 &centre, const Vector3 &radius)
	{
		return std::vector<GameObject *>();
	}*/

	/*std::vector<GameObject *> SceneStructure::QueryCube(const Vector3 &min, const Vector3 &max)
	{
		return std::vector<GameObject *>();
	}*/

	bool SceneStructure::Contains(GameObject *object)
	{
		for (auto &object2 : m_objects)
		{
			if (object2.get() == object)
			{
				return true;
			}
		}

		return false;
	}
}
