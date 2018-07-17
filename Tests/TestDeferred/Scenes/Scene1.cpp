#include "Scene1.hpp"

#include <Animations/MaterialAnimated.hpp>
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
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include <Physics/ColliderBox.hpp>
#include <Physics/ColliderSphere.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Physics/ColliderConvexHull.hpp>
#include <Physics/ColliderCylinder.hpp>
#include <Models/Shapes/ModelCylinder.hpp>
#include <Physics/ColliderCone.hpp>
#include <Scenes/Scenes.hpp>
#include <Models/Obj/ModelObj.hpp>
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"

namespace test
{
	static const float UI_SLIDE_TIME = 0.2f;

	Scene1::Scene1() :
		IScene(std::make_shared<FpsCamera>()),
		m_buttonSpawnSphere(new ButtonMouse({WSI_MOUSE_BUTTON_1})),
		m_buttonFullscreen(new ButtonKeyboard({WSI_KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({WSI_KEY_M, WSI_KEY_ESCAPE})),
		m_buttonScreenshot(new ButtonKeyboard({WSI_KEY_F12})),
		m_buttonExit(new ButtonKeyboard({WSI_KEY_DELETE})),
		m_soundScreenshot(new Sound("Sounds/Screenshot.ogg")),
		m_primaryColour(new Colour("#e74c3c")),
		m_selectorJoystick(new SelectorJoystick(WSI_JOYSTICK_1, 0, 1, 0, 1)),
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
		// Player.
		GameObject *playerObject = new GameObject("Objects/Player/Player.json", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		GameObject *skyboxObject = new GameObject("Objects/SkyboxChapel/SkyboxChapel.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated.
		GameObject *animatedObject = new GameObject(Transform());
		animatedObject->SetName("Animated");
		animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.json");
		animatedObject->AddComponent<MaterialAnimated>();
		animatedObject->AddComponent<MeshRender>();
		//animatedObject->AddComponent<ShadowRender>();

		// Entities.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 1000.0f, 8000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE, -1.0f);

		GameObject *plane = new GameObject(Transform(Vector3(0.0f, -5.0f, 0.0f), Vector3(), Vector3(200.0f, 3.0f, 200.0f)));
		plane->AddComponent<Mesh>(ModelCube::Resource(1.0f, 1.0f, 1.0f));
		plane->AddComponent<ColliderBox>(Vector3(1.0f, 1.0f, 1.0f));
		plane->AddComponent<Rigidbody>(0.0f, 0.5f);
		plane->AddComponent<MaterialDefault>(Colour::GREY, Texture::Resource("Undefined.png"), 0.0f, 1.0f);
		plane->AddComponent<MeshRender>();

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				GameObject *sphere = new GameObject(Transform(Vector3(6.7f * i, 6.7f * j, -8.0f), Vector3(), 3.0f));
				sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
				sphere->AddComponent<ColliderSphere>();
				sphere->AddComponent<Rigidbody>(1.5f);
				sphere->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Testing/Diffuse.png"),
					(float) j / 4.0f, (float) i / 4.0f, Texture::Resource("Objects/Testing/Material.png"), Texture::Resource("Objects/Testing/Normal.png"));
				sphere->AddComponent<MeshRender>();
			//	sphere->AddComponent<ShadowRender>();
			}
		}

		GameObject *convex = new GameObject(Transform(Vector3(27.0f, 3.0f, 48.0f), Vector3(), 1.2f));
		convex->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		convex->AddComponent<ColliderConvexHull>();
		convex->AddComponent<Rigidbody>(1.0f);
		convex->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
		convex->AddComponent<MeshRender>();
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere->WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();
			GameObject *sphere = new GameObject(Transform(cameraPosition, Vector3(), 3.0f));
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 1.0f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(1.5f);
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			rigidbody->AddForce((cameraRotation.ToQuaternion() * Vector3::FRONT) * 6000.0f, Vector3::ZERO);
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
			Screenshot::Capture(filename);
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
