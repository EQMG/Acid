#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Physics/ColliderBox.hpp>
#include <Physics/ColliderSphere.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Physics/ColliderConvexHull.hpp>
#include <Scenes/Scenes.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Renderer/Renderer.hpp>
#include <Physics/ColliderHeightfield.hpp>
#include <Terrain/Terrain.hpp>
#include <Terrain/MaterialTerrain.hpp>
#include <Physics/ColliderCapsule.hpp>
#include "FpsCamera.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FpsCamera()),
		m_buttonSpawnSphere(std::make_shared<ButtonMouse>(std::vector<MouseButton>{MOUSE_BUTTON_1})),
		m_buttonFullscreen(std::make_shared<ButtonKeyboard>(std::vector<Key>{KEY_F11})),
		m_buttonCaptureMouse(std::make_shared<ButtonKeyboard>(std::vector<Key>{KEY_M, KEY_ESCAPE})),
		m_buttonScreenshot(std::make_shared<ButtonKeyboard>(std::vector<Key>{KEY_F12})),
		m_buttonExit(std::make_shared<ButtonKeyboard>(std::vector<Key>{KEY_DELETE})),
		m_soundScreenshot(Sound("Sounds/Screenshot.ogg")),
		m_primaryColour(Colour("#e74c3c")),
		m_selectorJoystick(SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_uiStartLogo(new UiStartLogo(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
	}

	Scene1::~Scene1()
	{
		delete m_uiStartLogo;
		delete m_overlayDebug;
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));
		GetPhysics()->SetAirDensity(1.0f);

		// Player.
		auto playerObject = GameObject::Resource("Objects/Player/Player.xml", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		auto skyboxObject = GameObject::Resource("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated.
#ifdef ACID_BUILD_WINDOWS
		auto animatedObject = GameObject::Resource(Transform(Vector3(0.0f, 2.0f, 0.0f), Vector3(), 0.25f));
		animatedObject->SetName("Animated");
	//	animatedObject->AddComponent<ColliderCapsule>(0.23f, 1.3f);
		animatedObject->AddComponent<Rigidbody>(0.1f, 0.7f);
		animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
		animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
		animatedObject->AddComponent<MeshRender>();
	//	animatedObject->AddComponent<ShadowRender>();
#endif

		// Entities.
		auto sun = GameObject::Resource(Transform(Vector3(1000.0f, 5000.0f, -4000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE);

		auto terrain = GameObject::Resource(Transform());
		terrain->AddComponent<Terrain>(150.0f, 2.0f);
		terrain->AddComponent<ColliderHeightfield>();
		terrain->AddComponent<Rigidbody>(0.0f, 0.7f);
		terrain->AddComponent<Mesh>();
		terrain->AddComponent<MaterialTerrain>(Texture::Resource("Objects/Terrain/Grass.png"), Texture::Resource("Objects/Terrain/Rocks.png"));
		terrain->AddComponent<MeshRender>();
	//	terrain->AddComponent<ShadowRender>();

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto sphere = GameObject::Resource(Transform(Vector3(i, j + 0.5f, -10.0f), Vector3(), 0.5f));
				sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
				sphere->AddComponent<ColliderSphere>();
				sphere->AddComponent<Rigidbody>(0.5f);
				sphere->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Testing/Diffuse.png"),
					(float) j / 4.0f, (float) i / 4.0f, Texture::Resource("Objects/Testing/Material.png"), Texture::Resource("Objects/Testing/Normal.png"));
				sphere->AddComponent<MeshRender>();
			//	sphere->AddComponent<ShadowRender>();
			}
		}

		auto teapot = GameObject::Resource(Transform(Vector3(7.0f, 1.0f, 10.0f), Vector3(), 0.2f));
		teapot->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		teapot->AddComponent<ColliderConvexHull>();
		teapot->AddComponent<Rigidbody>(1.0f);
		teapot->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
		teapot->AddComponent<MeshRender>();
	//	teapot->AddComponent<ShadowRender>();

	//	auto system1 = GameObject::Create("Objects/Smoke/Smoke.json", Transform(Vector3(0.0f, 10.0f, 0.0f)));
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere->WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GameObject::Resource(Transform(cameraPosition, Vector3(), 0.5f));
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();
			rigidbody->AddForce<Force>((cameraRotation.ToQuaternion() * Vector3::FRONT).Normalize() * Vector3(-1.0f, 1.0f, -1.0f) * 3.0f, 2.0f);
			sphere->AddComponent<Light>(Colour::AQUA, 4.0f, Vector3(0.0f, 0.7f, 0.0f));
		}

		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse->WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot->WasDown())
		{
			m_soundScreenshot.Play();
			Renderer::Get()->CaptureScreenshot("Screenshots/" + Engine::Get()->GetDateTime() + ".png");
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}

		if (m_uiStartLogo->GetAlpha() == 0.0f && m_uiStartLogo->IsStarting())
		{
			m_uiStartLogo->SetAlphaDriver<DriverConstant>(0.0f);
			m_overlayDebug->SetAlphaDriver<DriverSlide>(0.0f, 1.0f, UI_SLIDE_TIME);
			m_uiStartLogo->SetStarting(false);
			Mouse::Get()->SetCursorHidden(true);
		}
	}

	bool Scene1::IsGamePaused() const
	{
		return m_uiStartLogo->IsStarting();
	}
}
