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
#include <Particles/ParticleType.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Voxels/Chunk.hpp>
#include <Voxels/VoxelRender.hpp>
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
		playerObject->SetName("PlayerFps");
		playerObject->AddComponent(new FpsPlayer());

		// Skybox.
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->SetName("SkyboxStars");
		skyboxObject->AddComponent(new Mesh(ShapeSphere::Resource(6, 6, 1.0f)));
		skyboxObject->AddComponent(new SkyboxRender(Cubemap::Resource("Resources/Skyboxes/Stars", ".png")));

		// Terrains.
		/*const int n = 4;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *terrainObject = new GameObject(Transform(Vector3(
					static_cast<float>(j) * TerrainRender::SIDE_LENGTH, 0.0f,
					static_cast<float>(w) * TerrainRender::SIDE_LENGTH)));
				terrainObject->SetName("Terrain");
				terrainObject->AddComponent(new Mesh());
			//	terrainObject->AddComponent(new Material());
				terrainObject->AddComponent(new Rigidbody());
				terrainObject->AddComponent(new LodBehaviour());
				terrainObject->AddComponent(new TerrainRender());
			//	terrainObject->AddComponent(new ShadowRender());
			}
		}*/

		// Chunks.
		const int n = 1;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *chunk = new GameObject(Transform(2.0f * Chunk::SIDE_LENGTH * Vector3(j, -0.25f, w)));
				chunk->SetName("Chunk" + j + w);
				chunk->AddComponent(new Chunk());
				chunk->AddComponent(new Mesh());
				chunk->AddComponent(new VoxelRender());
			}
		}

		// Waters.
		GameObject *waterObject = new GameObject(Transform(Vector3(), Vector3()));
		waterObject->SetName("Water");
		waterObject->AddComponent(new Mesh(new MeshWater()));
		waterObject->AddComponent(new WaterRender());

		// Entities.
		new GameObject("Sun", Transform(Vector3(), Vector3(), 18.0f));
		new GameObject("Moon", Transform(Vector3(), Vector3(), 9.0f));

		/*Vector3 foundationCentre = Terrains::Get()->GetPosition(15.7f, -25.0f);

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

				if (position.m_y >= 0.0f)
				{
					if (i == 0 && j == 0)
					{
						new GameObject("GemPurple", Transform(position, Vector3(), 1.0f));
					}
					else if (i < 0)
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
		}*/

		// Music.
/*#ifdef FLOUNDER_CONFIG_RELEASE
		Sound *music1 = new Sound("Resources/Music/Outpost.ogg", 0.9f);
		//music1->SetPosition(Vector3(0.0f, 7.0f, 0.0f));
		music1->Loop();
#endif*/

		/*GameObject *particlesObject = new GameObject(Transform(Vector3(0.0f, 20.0f, 0.0f)));
		std::vector<ParticleType*> *types = new std::vector<ParticleType*>();
		types->push_back(new ParticleType("blue", Texture::Resource("Resources/Particles/Blue/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("purple", Texture::Resource("Resources/Particles/Purple/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("red", Texture::Resource("Resources/Particles/Red/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("yellow", Texture::Resource("Resources/Particles/Yellow/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		particlesObject->AddComponent(new ParticleSystem(types, new SpawnCircle(30.0f, Vector3(0.0f, 10.0f, 0.0f)), 1000, 1.0f, -0.1f));*/
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
