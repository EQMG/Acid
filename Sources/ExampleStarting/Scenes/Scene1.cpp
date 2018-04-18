#include "Scene1.hpp"

#include <Inputs/Buttons/ButtonKeyboard.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ShapeSphere.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Lights/Light.hpp>
#include <Entities/MaterialEntity.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Meshes/MeshRender.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include "ManagerUis.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"

namespace Demo
{
	Scene1::Scene1() :
		Scene(new FpsCamera(), new ManagerUis()),
		m_buttonFullscreen(new ButtonKeyboard({GLFW_KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({GLFW_KEY_M})),
		m_buttonScreenshot(new ButtonKeyboard({GLFW_KEY_F12})),
		m_buttonExit(new ButtonKeyboard({GLFW_KEY_DELETE})),
		m_soundScreenshot(new Sound("Resources/Sounds/Screenshot.ogg"))
	{
	}

	Scene1::~Scene1()
	{
		delete m_buttonFullscreen;
		delete m_buttonCaptureMouse;
		delete m_buttonScreenshot;
		delete m_buttonExit;
	}

	void Scene1::Start()
	{
		//	// Camera.
		//	GameObject *cameraObject = new GameObject(Transform(Vector3(), Vector3(), 1.0f));
		//	cameraObject->SetName("Camera");
		//	cameraObject->AddComponent<FpsCamera>();

		// Player.
		GameObject *playerObject = new GameObject(Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f), 1.0f));
		playerObject->SetName("Player");
		playerObject->AddComponent<FpsPlayer>();

		// Skybox.
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->SetName("SkyboxChapel");
		skyboxObject->AddComponent<Mesh>(ShapeSphere::Resource(6, 6, 1.0f));
		skyboxObject->AddComponent<SkyboxRender>(Cubemap::Resource("Resources/Skyboxes/Chapel", ".png"), false);

		// Entities.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 1000.0f, 8000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour("#FFFFFF"), -1.0f);

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				GameObject *sphere = new GameObject(Transform(Vector3(6.7f * i, 6.7f * j, -8.0f), Vector3(), 3.0f));
				sphere->AddComponent<Mesh>(ShapeSphere::Resource(30, 30, 1.0f));
				sphere->AddComponent<MaterialEntity>(Colour("#ffffff"), Texture::Resource("Resources/Entities/Testing/Diffuse.png"),
					(float) j / 4.0f, (float) i / 4.0f, Texture::Resource("Resources/Entities/Testing/Material.png"), Texture::Resource("Resources/Entities/Testing/Normal.png"));
				sphere->AddComponent<MeshRender>();
				sphere->AddComponent<ShadowRender>();
			}
		}
	}

	void Scene1::Update()
	{
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
	}
}
