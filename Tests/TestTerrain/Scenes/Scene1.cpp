#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ShapeSphere.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include <Waters/MeshWater.hpp>
#include "Terrains/MeshTerrain.hpp"
#include "Terrains/LodBehaviour.hpp"
#include "Terrains/MaterialTerrain.hpp"
#include "Voxels/VoxelChunk.hpp"
#include "Voxels/MaterialVoxel.hpp"
#include "Waters/MaterialWater.hpp"
#include "ManagerUis.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"

namespace test
{
	Scene1::Scene1() :
		IScene(new FpsCamera(), new ManagerUis()),
		m_buttonFullscreen(new ButtonKeyboard({Key::KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({Key::KEY_M, Key::KEY_ESCAPE})),
		m_buttonScreenshot(new ButtonKeyboard({Key::KEY_F12})),
		m_buttonExit(new ButtonKeyboard({Key::KEY_DELETE})),
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
		new GameObject("Player", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		new GameObject("SkyboxStars", Transform(Vector3(), Vector3(), 2048.0f));

		// Objects.
		new GameObject("Sun", Transform(Vector3(), Vector3(), 18.0f));
		new GameObject("Moon", Transform(Vector3(), Vector3(), 9.0f));

		// Terrains.
		const int n = 4;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *terrainChunk = new GameObject(Transform(Vector3(
					static_cast<float>(j) * MeshTerrain::SIDE_LENGTH, 0.0f,
					static_cast<float>(w) * MeshTerrain::SIDE_LENGTH)));
				terrainChunk->SetName("Terrain");
				terrainChunk->AddComponent<Mesh>();
				terrainChunk->AddComponent<LodBehaviour>();
				terrainChunk->AddComponent<MaterialTerrain>();
				terrainChunk->AddComponent<MeshRender>();
			}
		}

		// Waters.
		GameObject *water = new GameObject(Transform());
		water->SetName("Water");
		water->AddComponent<Mesh>(std::make_shared<MeshWater>());
		water->AddComponent<MaterialWater>();
		water->AddComponent<MeshRender>();

		/*// Voxels.
		GameObject *voxelChunk = new GameObject(Transform());
		voxelChunk->SetName("Chunk_0_0");
		voxelChunk->AddComponent<Mesh>();
		voxelChunk->AddComponent<MaterialVoxel>();
		voxelChunk->AddComponent<VoxelChunk>(MESH_GREEDY, true);
		voxelChunk->AddComponent<MeshRender>();*/
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
