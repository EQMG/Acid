#include "Scene.hpp"

namespace Flounder
{
	Scene::Scene(ICamera *camera) :
		m_camera(camera),
		m_structure(new StructureBasic<GameObject *>())
	{
	}

	Scene::~Scene()
	{
		delete m_camera;
		delete m_structure;
	}
}
