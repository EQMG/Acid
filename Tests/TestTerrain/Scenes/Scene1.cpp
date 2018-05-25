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
		Scene(new FpsCamera(), new ManagerUis()),
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
		// GameObject *playerObject = new GameObject("Player", Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));
		GameObject *playerObject = new GameObject(Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f), 1.0f));
		playerObject->SetName("Player");
		playerObject->AddComponent<FpsPlayer>();
	//	playerObject->AddComponent<MeshAnimated>("Resources/Objects/Player/Model.json");
	//	playerObject->AddComponent<MaterialDefault>();
	//	playerObject->AddComponent<MeshRender>();

		// Skybox.
		// GameObject *skyboxObject = new GameObject("SkyboxChapel", Transform(Vector3(), Vector3(), 2048.0f));
		GameObject *skyboxObject = new GameObject(Transform(Vector3(), Vector3(), 2048.0f));
		skyboxObject->SetName("SkyboxClouds");
		skyboxObject->AddComponent<Mesh>(ShapeSphere::Resource(6, 6, 1.0f));
		skyboxObject->AddComponent<MaterialSkybox>(Cubemap::Resource("Resources/Objects/SkyboxClouds", ".png"), false);
		skyboxObject->AddComponent<MeshRender>();

		// Objects.
		GameObject *sun = new GameObject(Transform(Vector3(100.0f, 1000.0f, 8000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour("#FFFFFF"), -1.0f);

		/*// Voxels.
		GameObject *voxelChunk = new GameObject(Transform());
		voxelChunk->SetName("Chunk_0_0");
		voxelChunk->AddComponent<Mesh>();
		voxelChunk->AddComponent<MaterialVoxel>();
		voxelChunk->AddComponent<VoxelChunk>(MESH_GREEDY, true);
		voxelChunk->AddComponent<MeshRender>();*/

		// Terrains.
		GameObject *terrainChunk = new GameObject(Transform());
		terrainChunk->SetName("Terrain");
		terrainChunk->AddComponent<Mesh>();
		terrainChunk->AddComponent<LodBehaviour>(0.0f, Transform());
		terrainChunk->AddComponent<MaterialTerrain>();
		terrainChunk->AddComponent<MeshRender>();

		/*// Waters.
		GameObject *water = new GameObject(Transform());
		water->SetName("Water");
		water->AddComponent<Mesh>();
		water->AddComponent<MaterialWater>();
		water->AddComponent<MeshRender>();*/
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
