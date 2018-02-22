#include "Scene.hpp"

namespace Flounder
{
	Scene::Scene() :
		m_structure(new StructureBasic<GameObject *>())
	{
	}

	Scene::~Scene()
	{
		delete m_structure;
	}
}
