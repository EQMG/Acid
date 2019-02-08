#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>

namespace test
{
	Scene1::Scene1() :
		Scene(new Camera())
	{
	}

	void Scene1::Start()
	{
	}

	void Scene1::Update()
	{
	}

	bool Scene1::IsPaused() const
	{
		return true;
	}
}
