#include "SceneStructure.hpp"

#include "Physics/Rigidbody.hpp"

namespace acid
{
SceneStructure::SceneStructure()
{
}

Entity *SceneStructure::GetEntity(const std::string &name) const
{
	for (auto &object : m_objects)
	{
		if (object->GetName() == name)
		{
			return object.get();
		}
	}

	return nullptr;
}

Entity *SceneStructure::CreateEntity()
{
	return m_objects.emplace_back(std::make_unique<Entity>()).get();
}

Entity *SceneStructure::CreateEntity(const std::string &filename)
{
	return m_objects.emplace_back(std::make_unique<Entity>(filename)).get();
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
	for (auto it{--m_objects.end()}; it != m_objects.begin(); --it)
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
	for (auto it{m_objects.begin()}; it != m_objects.end();)
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
	std::vector<Entity *> entities;

	for (const auto &object : m_objects)
	{
		if (object->IsRemoved())
		{
			continue;
		}

		entities.emplace_back(object.get());
	}

	return entities;
}

std::vector<Entity *> SceneStructure::QueryFrustum(const Frustum &range)
{
	std::vector<Entity *> entities;

	for (const auto &object : m_objects)
	{
		if (object->IsRemoved())
		{
			continue;
		}

		auto rigidbody{object->GetComponent<Rigidbody>()};

		if (rigidbody == nullptr || rigidbody->InFrustum(range))
		{
			entities.emplace_back(object.get());
		}
	}

	return entities;
}

/*std::vector<Entity *> SceneStructure::QuerySphere(const Vector3 &centre, const Vector3 &radius)
{
	return {};
}*/

/*std::vector<Entity *> SceneStructure::QueryCube(const Vector3 &min, const Vector3 &max)
{
	return {};
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
