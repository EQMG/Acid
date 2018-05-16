#include "Scene.hpp"

namespace fl
{
	Scene::Scene(ICamera *camera, IManagerUis *managerUis) :
		m_camera(camera),
		m_managerUis(managerUis),
		m_structure(new SceneStructure())
	{
	}

	Scene::~Scene()
	{
		delete m_camera;
		delete m_managerUis;
		delete m_structure;
	}
}
