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
#include "FpsCamera.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(new FpsCamera()),
		m_buttonSpawnSphere(new ButtonMouse({MOUSE_BUTTON_1})),
		m_buttonFullscreen(new ButtonKeyboard({KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({KEY_M, KEY_ESCAPE})),
		m_buttonScreenshot(new ButtonKeyboard({KEY_F12})),
		m_buttonExit(new ButtonKeyboard({KEY_DELETE})),
		m_soundScreenshot(new Sound("Sounds/Screenshot.ogg")),
		m_primaryColour(new Colour("#e74c3c")),
		m_selectorJoystick(new SelectorJoystick(JOYSTICK_1, 0, 1, 0, 1)),
		m_uiStartLogo(new UiStartLogo(Uis::Get()->GetContainer())),
		m_overlayDebug(new OverlayDebug(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
	}

	Scene1::~Scene1()
	{
		delete m_buttonSpawnSphere;
		delete m_buttonFullscreen;
		delete m_buttonCaptureMouse;
		delete m_buttonScreenshot;
		delete m_buttonExit;

		delete m_primaryColour;
		delete m_selectorJoystick;
		delete m_uiStartLogo;
		delete m_overlayDebug;
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));
		GetPhysics()->SetAirDensity(1.0f);

		// Player.
		GameObject *playerObject = new GameObject("Objects/Player/Player.xml", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		GameObject *skyboxObject = new GameObject("Objects/SkyboxChapel/SkyboxChapel.json", Transform(Vector3(), Vector3(), 2048.0f));
	//	skyboxObject->GetComponent<MeshRender>()->SetEnabled(false);

		// Animated.
	//	GameObject *animatedObject = new GameObject(Transform(Vector3(), Vector3(), 0.25f));
	//	animatedObject->SetName("Animated");
	//	animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
	//	animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Albedo.png"), 0.7f, 0.6f);
	//	animatedObject->AddComponent<MeshRender>();
	//	animatedObject->AddComponent<ShadowRender>();

		// Entities.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 1000.0f, 8000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE, -1.0f);

		GameObject *plane = new GameObject(Transform(Vector3(0.0f, -0.5f, 0.0f), Vector3(), Vector3(50.0f, 1.0f, 50.0f)));
		plane->AddComponent<Mesh>(ModelCube::Resource(1.0f, 1.0f, 1.0f));
		plane->AddComponent<ColliderBox>(Vector3(1.0f, 1.0f, 1.0f));
		plane->AddComponent<Rigidbody>(0.0f, 0.5f);
		plane->AddComponent<MaterialDefault>(Colour::GREY, Texture::Resource("Undefined2.png"), 0.0f, 1.0f);
		plane->AddComponent<MeshRender>();
		plane->AddComponent<ShadowRender>();

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				GameObject *sphere = new GameObject(Transform(Vector3(i, j + 0.5f, -10.0f), Vector3(), 0.5f));
				sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
				sphere->AddComponent<ColliderSphere>();
				sphere->AddComponent<Rigidbody>(0.5f);
				sphere->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Testing/Albedo.png"),
					(float) j / 4.0f, (float) i / 4.0f, Texture::Resource("Objects/Testing/Material.png"), Texture::Resource("Objects/Testing/Normal.png"));
				sphere->AddComponent<MeshRender>();
				sphere->AddComponent<ShadowRender>();
			}
		}

		GameObject *convex = new GameObject(Transform(Vector3(7.0f, 1.0f, 10.0f), Vector3(), 0.2f));
		convex->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		convex->AddComponent<ColliderConvexHull>();
		convex->AddComponent<Rigidbody>(1.0f);
		convex->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
		convex->AddComponent<MeshRender>();
		convex->AddComponent<ShadowRender>();

		/*auto system1Types = std::vector<std::shared_ptr<ParticleType>>{
			ParticleType::Resource(Texture::Resource("Particles/Circular.png"), 4, Colour::BLUE, 10.0f, 1.0f),
			ParticleType::Resource(Texture::Resource("Particles/Circular.png"), 4, Colour::YELLOW, 6.0f, 3.0f),
			ParticleType::Resource(Texture::Resource("Particles/Circular.png"), 4, Colour::RED, 8.0f, 2.0f)
		};
		GameObject *system1 = new GameObject(Transform(Vector3(0.0f, 10.0f, 0.0f)));
		system1->AddComponent<ParticleSystem>(system1Types, std::make_shared<SpawnCircle>(2.0f, Vector3::UP), 50.0f, 0.3f, 1.0f, Vector3::ZERO);*/
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere->WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
			GameObject *sphere = new GameObject(Transform(cameraPosition, Vector3(), 0.5f));
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			rigidbody->AddForce<Force>((cameraRotation.ToQuaternion() * Vector3::FRONT).Normalize() * Vector3(-1.0f, 1.0f, -1.0f) * 3.0f, 2.0f);
		}

		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse->WasDown())
		{
			//	Scenes::Get()->SetScene(new Scene2());
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot->WasDown())
		{
			std::string filename = "Screenshots/" + Engine::Get()->GetDateTime() + ".png";

			m_soundScreenshot->Play();
			Renderer::Get()->CaptureScreenshot(filename);
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

	bool Scene1::IsGamePaused()
	{
		return m_uiStartLogo->IsStarting();
	}
}
