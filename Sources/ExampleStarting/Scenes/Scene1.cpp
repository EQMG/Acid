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
#include <Materials/MaterialDefault.hpp>
#include <Particles/ParticleType.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Voxels/VoxelChunk.hpp>
#include <Voxels/VoxelRender.hpp>
#include <Lights/Light.hpp>
#include <Entities/EntityRender.hpp>
#include <Models/Shapes/ShapeCube.hpp>
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
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 6048.0f));
		skyboxObject->SetName("SkyboxStars");
		skyboxObject->AddComponent<Mesh>(ShapeCube::Resource(1.0f, 1.0f, 1.0f)); // ShapeSphere::Resource(6, 6, 1.0f)
		skyboxObject->AddComponent<SkyboxRender>(Cubemap::Resource("Resources/Skyboxes/Stars", ".png"), false);

		// Terrains.
	//	new GameObject("PlanetCentre", Transform(Vector3(), Vector3(), 600.0f));
		int n = 1;
		float radius = ((2 * n + 1) * TerrainRender::SIDE_LENGTH) / 2.0f;
		float side = TerrainRender::SIDE_LENGTH;

		for (int j = -n; j < n; j++)
		{
			for (int w = -n; w < n; w++)
			{
				CreateChunk(radius, Transform(Vector3(j * side, radius, w * side), Vector3(0.0f, 0.0f, 0.0f), side)); // Top.
				CreateChunk(radius, Transform(Vector3(j * side, -radius, w * side), Vector3(180.0f, 0.0f, 0.0f), side)); // Bottom.
				CreateChunk(radius, Transform(Vector3(w * side, j * side, radius), Vector3(90.0f, 0.0f, 0.0f), side)); // Back.
				CreateChunk(radius, Transform(Vector3(w * side, j * side, -radius), Vector3(270.0f, 0.0f, 0.0f), side)); // Front.
				CreateChunk(radius, Transform(Vector3(radius, j * side, w * side), Vector3(0.0f, 0.0f, 270.0f), side)); // Right.
				CreateChunk(radius, Transform(Vector3(-radius, j * side, w * side), Vector3(0.0f, 0.0f, 90.0f), side)); // Left.
			}
		}

		/*const int n = 1;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				new GameObject("Terrain", Transform(Vector3(
					static_cast<float>(j) * TerrainRender::SIDE_LENGTH, 100.0f - ((j + w) * 100.0f),
					static_cast<float>(w) * TerrainRender::SIDE_LENGTH)));
			}
		}*/

		// Waters.
		/*GameObject *waterObject = new GameObject(Transform(Vector3(), Vector3()));
		waterObject->SetName("Water_0_0");
		waterObject->AddComponent<Mesh>(new MeshWater());
		waterObject->AddComponent<WaterRender>();*/

		// Chunks.
		/*const int n = 0;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *chunk = new GameObject(Transform(*Chunk::CHUNK_SIZE * Vector3(j, 0.0f, w)));
				chunk->SetName("Chunk_" + std::to_string(j) + "_" + std::to_string(w));
				chunk->AddComponent(new Chunk(MeshGreedy, true));
				chunk->AddComponent(new Mesh());
				chunk->AddComponent(new VoxelRender());
				//	chunk->AddComponent(new ShadowRender());
			}
		}*/

		// Entities.
		new GameObject("Sun", Transform(Vector3(), Vector3(), 230.0f));
		new GameObject("Moon", Transform(Vector3(), Vector3(), 190.0f));
	//	new GameObject("Player", Transform(Vector3(5, 20, 5), Vector3(), 1.0f));

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
		/*Sound *music1 = new Sound("Resources/Music/Outpost.ogg", 0.9f);
		music1->SetPosition(Vector3(0.0f, 15.0f, 0.0f));
		music1->Loop();*/

		// Particles.
		/*GameObject *particlesObject = new GameObject(Transform(Vector3(0.0f, 20.0f, 0.0f)));
		std::vector<ParticleType*> *types = new std::vector<ParticleType*>();
		types->push_back(new ParticleType("blue", Texture::Resource("Resources/Particles/Blue/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("purple", Texture::Resource("Resources/Particles/Purple/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("red", Texture::Resource("Resources/Particles/Red/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("yellow", Texture::Resource("Resources/Particles/Yellow/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		particlesObject->AddComponent<ParticleSystem>(types, new SpawnCircle(30.0f, Vector3(0.0f, 10.0f, 0.0f)), 1000, 1.0f, -0.1f);*/
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

	GameObject *Scene1::CreateChunk(const float &radius, const Transform &transform)
	{
		GameObject *chunk = new GameObject(Transform());
		chunk->SetName("Terrain");
		chunk->AddComponent(new Mesh());
		//	chunk->AddComponent(new Material());
		chunk->AddComponent(new Rigidbody());
		chunk->AddComponent(new LodBehaviour(radius, transform));
		chunk->AddComponent(new TerrainRender());
		//	chunk->AddComponent(new ShadowRender());
		return chunk;
	}
}
