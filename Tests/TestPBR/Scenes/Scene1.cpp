#include "Scene1.hpp"

#include <Files/FileSystem.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/ButtonJoystick.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Uis/Uis.hpp>
#include "CameraFps.hpp"
#include "PlayerFps.hpp"

namespace test
{
static const float UI_SLIDE_TIME = 0.2f;

Scene1::Scene1() :
	Scene(new CameraFps()),
	m_buttonPause(ButtonCompound({ new ButtonKeyboard(Key::Escape), new ButtonJoystick(0, 7) })),
	m_paused(false),
	m_overlayDebug(&Uis::Get()->GetCanvas())
{
	m_buttonPause.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			m_paused = !m_paused;
		}
	});
}

void Scene1::Start()
{
	GetPhysics()->SetGravity(Vector3f(0.0f, -9.81f, 0.0f));
	GetPhysics()->SetAirDensity(1.0f);

	// Player.
	auto playerObject = GetStructure()->CreateEntity(Transform(Vector3f(), Vector3f(0.0f, 180.0f, 0.0f)));
	//playerObject->AddComponent<Rigidbody>(1.0f, 0.4f, Transform::ZERO, Vector3::ZERO, Vector3::ZERO);
	//playerObject->AddComponent<ColliderCapsule>(0.2f, 1.8f);
	playerObject->AddComponent<PlayerFps>();

	// Skybox.
	auto skyboxObject = GetStructure()->CreateEntity("Objects/SkyboxSnowy/SkyboxSnowy.json", Transform(Vector3f(), Vector3f(), 1024.0f));

	// Entities.
	auto sun = GetStructure()->CreateEntity(Transform(Vector3f(1000.0f, 5000.0f, 4000.0f), Vector3f(), 18.0f));
	sun->AddComponent<Light>(Colour::White);

	for (uint32_t i = 0; i < 6; i++)
	{
		for (uint32_t j = 0; j < 6; j++)
		{
			auto sphere = GetStructure()->CreateEntity(Transform(Vector3f(i, j, -6.0f), Vector3f(), 0.5f));
			sphere->AddComponent<Mesh>(ModelSphere::Create(1.0f, 30, 30));
			sphere->AddComponent<MaterialDefault>(Colour::Red, Image2d::Create("Objects/Testing/Diffuse.png"), j / 5.0f, i / 5.0f,
				nullptr, // Image2d::Create("Objects/Testing/Material.png")
				Image2d::Create("Objects/Testing/Normal.png"));
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();

			auto teapot = GetStructure()->CreateEntity(Transform(Vector3f(i * 1.6f, j, 6.0f), Vector3f(), 0.14f));
			teapot->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Tea.obj"));
			teapot->AddComponent<MaterialDefault>(Colour::White, nullptr, j / 5.0f, i / 5.0f);
			teapot->AddComponent<MeshRender>();
			teapot->AddComponent<ShadowRender>();
		}
	}

	/*auto dragon = GetStructure()->CreateEntity(Transform(Vector3(6.0f, 0.0f, 0.0f), Vector3(0.0f, -90.0f, 0.0f), 0.4f));
	dragon->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Dragon.obj"));
	dragon->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.7f, 0.1f);
	dragon->AddComponent<MeshRender>();
	dragon->AddComponent<ShadowRender>();*/
}

void Scene1::Update()
{
}

bool Scene1::IsPaused() const
{
	return m_paused;
}
}
