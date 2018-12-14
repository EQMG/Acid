#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Helpers/FileSystem.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Objects/Prefabs/PrefabObject.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Uis/Uis.hpp>
#include "Voxels/MaterialVoxel.hpp"
#include "Voxels/VoxelChunk.hpp"
#include "CameraFps.hpp"
#include "PlayerFps.hpp"

namespace test
{
	static const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

	Scene1::Scene1() :
		Scene(new CameraFps(), new SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_buttonFullscreen(ButtonKeyboard({KEY_F11})),
		m_buttonCaptureMouse(ButtonKeyboard({KEY_M, KEY_ESCAPE})),
		m_buttonScreenshot(ButtonKeyboard({KEY_F12})),
		m_buttonExit(ButtonKeyboard({KEY_DELETE})),
		m_soundScreenshot(Sound("Sounds/Screenshot.ogg")),
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant>(0.0f);
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));
		GetPhysics()->SetAirDensity(1.0f);

		// Player.
		auto playerObject = new Entity(Transform(Vector3(), Vector3(0.0f, 180.0f, 0.0f)));
		playerObject->AddComponent<PlayerFps>();

		// Skybox.
		auto skyboxObject = new Entity("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Entities.
		auto sun = new Entity(Transform(Vector3(1000.0f, 5000.0f, -4000.0f), Vector3(), 18.0f));
		sun->AddComponent<Light>(Colour::WHITE);

		// Voxels.
		for (int32_t x = -3; x <= 3; x++)
		{
			for (int32_t z = -3; z <= 3; z++)
			{
				for (int32_t y = -1; y <= 1; y++)
				{
					auto voxelChunk = new Entity(Transform(VoxelChunk::CHUNK_SIZE * Vector3(x, y, z)));
					voxelChunk->SetName("Chunk_" + String::To(x) + "_" + String::To(y) + "_" + String::To(z));
					voxelChunk->AddComponent<Mesh>();
					voxelChunk->AddComponent<VoxelChunk>(MESHER_CULLED, true);
					voxelChunk->AddComponent<MaterialVoxel>();
					voxelChunk->AddComponent<MeshRender>();
				}
			}
		}
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen.WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse.WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot.WasDown())
		{
			m_soundScreenshot.Play();
			Renderer::Get()->CaptureScreenshot(FileSystem::GetWorkingDirectory() + "/Screenshots/" + Engine::GetDateTime() + ".png");
		}

		if (m_buttonExit.WasDown())
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

	bool Scene1::IsPaused() const
	{
		return m_uiStartLogo->IsStarting();
	}
}
