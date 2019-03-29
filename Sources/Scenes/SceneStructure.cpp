#include "SceneStructure.hpp"

#include "Physics/Rigidbody.hpp"

namespace acid
{
SceneStructure::SceneStructure()
{
}

Entity *SceneStructure::CreateEntity(const Transform &transform)
{
	auto entity = new Entity(transform);
	m_objects.emplace_back(entity);
	return entity;
}

Entity *SceneStructure::CreateEntity(const std::string &filename, const Transform &transform)
{
	auto entity = new Entity(filename, transform);
	m_objects.emplace_back(entity);
	return entity;
}

void SceneStructure::Add(Entity *object)
{
	m_objects.emplace_back(object);
}

void SceneStructure::Add(std::unique_ptr<Entity> object)
{
	m_objects.emplace_back(std::move(object));
}

void SceneStructure::Remove(Entity *object)
{
	m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [object](std::unique_ptr<Entity> &e)
	{
		return e.get() == object;
	}), m_objects.end());
}

void SceneStructure::Move(Entity *object, SceneStructure &structure)
{
	for (auto it = --m_objects.end(); it != m_objects.begin(); --it)
	{
		if ((*it).get() != object)
		{
			continue;
		}

		structure.Add(std::move(*it));
		m_objects.erase(it);
	}
}

void SceneStructure::Clear()
{
	m_objects.clear();
}

void SceneStructure::Update()
{
	for (auto it = m_objects.begin(); it != m_objects.end();)
	{
		if ((*it)->IsRemoved())
		{
			it = m_objects.erase(it);
			continue;
		}

		(*it)->Update();
		++it;
	}
}

std::vector<Entity *> SceneStructure::QueryAll()
{
	std::vector<Entity *> result = {};

	for (const auto &object : m_objects)
	{
		if (object->IsRemoved())
		{
			continue;
		}

		result.emplace_back(object.get());
	}

	return result;
}

std::vector<Entity *> SceneStructure::QueryFrustum(const Frustum &range)
{
	std::vector<Entity *> result = {};

	for (const auto &object : m_objects)
	{
		if (object->IsRemoved())
		{
			continue;
		}

		auto rigidbody = object->GetComponent<Rigidbody>();

		if (rigidbody == nullptr || rigidbody->InFrustum(range))
		{
			result.emplace_back(object.get());
		}
	}

	return result;
}

/*std::vector<Entity *> SceneStructure::QuerySphere(const Vector3 &centre, const Vector3 &radius)
{
	return std::vector<Entity *>();
}*/

/*std::vector<Entity *> SceneStructure::QueryCube(const Vector3 &min, const Vector3 &max)
{
	return std::vector<Entity *>();
}*/

bool SceneStructure::Contains(Entity *object)
{
	for (const auto &object2 : m_objects)
	{
		if (object2.get() == object)
		{
			return true;
		}
	}

	return false;
}
}
