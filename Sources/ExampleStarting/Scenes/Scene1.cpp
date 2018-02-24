#include "Scene1.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Scenes/Scenes.hpp>
#include <Terrains/Terrains.hpp>
#include <Maths/Maths.hpp>
#include <Devices/Display.hpp>
#include <Devices/Mouse.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ShapeSphere.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Terrains/LodBehaviour.hpp>
#include <Waters/MeshWater.hpp>
#include <Waters/WaterRender.hpp>
#include <Materials/Material.hpp>
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
	//	cameraObject->AddComponent(new FpsCamera());

		// Player.
		GameObject *playerObject = new GameObject(Transform(Vector3(), Vector3(), 1.0f));
		playerObject->AddComponent(new FpsPlayer());

		// Skybox.
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->AddComponent(new Mesh(ShapeSphere::Resource(6, 6, 1.0f)));
		skyboxObject->AddComponent(new SkyboxRender(Cubemap::Resource("Resources/Skyboxes/Stars", ".png")));

		// Terrains.
		const int n = 4;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *terrainObject = new GameObject(Transform(Vector3(
					static_cast<float>(j) * TerrainRender::SIDE_LENGTH, 0.0f,
					static_cast<float>(w) * TerrainRender::SIDE_LENGTH)));
				terrainObject->AddComponent(new Mesh());
			//	terrainObject->AddComponent(new Material());
				terrainObject->AddComponent(new TerrainRender());
				terrainObject->AddComponent(new Rigidbody());
				terrainObject->AddComponent(new LodBehaviour());
			}
		}

		// Waters.
		GameObject *waterObject = new GameObject(Transform(Vector3(), Vector3()));
		waterObject->AddComponent(new Mesh(new MeshWater()));
		waterObject->AddComponent(new WaterRender());

		// Entities.
		new GameObject("Sun", Transform(Vector3(), Vector3(), 18.0f));
		new GameObject("Moon", Transform(Vector3(), Vector3(), 9.0f));

		Vector3 foundationCentre = Terrains::Get()->GetPosition(15.7f, -25.0f);

		for (int i = -3; i <= 0; i++)
		{
			for (int j = -3; j <= 0; j++)
			{
				new GameObject("FoundationWood", Transform(foundationCentre + Vector3(i * 3.0f, 0.7f, -j * 3.0f), Vector3(), 1.5f));
			}
		}

		for (int i = -8; i <= 0; i++)
		{
			for (int j = -8; j <= 0; j++)
			{
				const float xv = Maths::RandomInRange(-3.0f, 3.0f);
				const float yv = Maths::RandomInRange(-3.0f, 3.0f);
				Vector3 position = Terrains::Get()->GetPosition((13.0f * i) + xv, (13.0f * j) + yv);

				if (position.m_y > 0.0f)
				{
					if (i < 0)
					{
						position.m_y -= 1.0f;
						new GameObject("TreePine", Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), 1.0f));
					}
					else
					{
						position.m_y += 1.0f;
						new GameObject("Testing", Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), 1.0f));
					}
				}
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
