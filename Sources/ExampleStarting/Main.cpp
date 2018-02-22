#include <iostream>
#include <Camera/Camera.hpp>
#include <Devices/Mouse.hpp>
#include <Engine/ModuleUpdater.hpp>
#include <Helpers/FileSystem.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/ShapeSphere.hpp>
#include <Objects/Objects.hpp>
#include <Renderer/Renderer.hpp>
#include <Skyboxes/SkyboxRender.hpp>
#include <Space/StructureBasic.hpp>
#include <Standards/Standards.hpp>
#include <Terrains/LodBehaviour.hpp>
#include <Waters/MeshWater.hpp>
#include "Configs/ConfigManager.hpp"
#include "FpsCamera.hpp"
#include "FpsPlayer.hpp"
#include "Instance.hpp"
#include "ManagerRender.hpp"
#include "ManagerUis.hpp"

using namespace Demo;

//#if (FLOUNDER_CONFIG_RELEASE && FLOUNDER_PLATFORM_WINDOWS)
//int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
//#else
int main(int argc, char **argv)
//#endif
{
	// Creates the engine object.
	Engine *m_engine = new Engine();
	m_engine->SetUpdater(new ModuleUpdater());

	printf("Working Directory: %s\n", FileSystem::GetWorkingDirectory().c_str());
	ConfigManager *configManager = new ConfigManager();

	// Initializes the engine modules.
	if (Display::Get() != nullptr)
	{
		Display::Get()->SetTitle("Example Starting");
		Display::Get()->SetIcon("Resources/Logos/Tail.png");
	}

	if (Mouse::Get() != nullptr)
	{
		Mouse::Get()->SetCustomMouse("Resources/Guis/Cursor.png");
	}

	if (Camera::Get() != nullptr)
	{
		Camera::Get()->SetCamera(new FpsCamera());
		Camera::Get()->SetPlayer(new FpsPlayer());
	}

	if (Objects::Get() != nullptr)
	{
		Objects::Get()->SetStructure(new StructureBasic<GameObject *>()); // TODO: Move into scene.
	}

	if (Renderer::Get() != nullptr)
	{
		Renderer::Get()->SetManager(new ManagerRender());
	}

	if (Uis::Get() != nullptr)
	{
		Uis::Get()->SetManager(new ManagerUis());
	}

	if (Standards::get() != nullptr)
	{
		Standards::get()->AddStandard(new Instance());
	}

	if (Skyboxes::Get() != nullptr)
	{
		GameObject *object = new GameObject(Transform(Vector3(), Vector3(), 2048.0f), Objects::Get()->GetStructure());
		object->AddComponent(new Mesh(ShapeSphere::Resource(6, 6, 1.0f)));
		object->AddComponent(new SkyboxRender(Cubemap::Resource("Resources/Skyboxes/Stars", ".png")));
	}

	if (Terrains::Get() != nullptr)
	{
		const int n = 4;

		for (int j = -n; j <= n; j++)
		{
			for (int w = -n; w <= n; w++)
			{
				GameObject *object = new GameObject(Transform(Vector3(
					2.0f * static_cast<float>(j) * TerrainRender::SIDE_LENGTH, 0.0f,
					2.0f * static_cast<float>(w) * TerrainRender::SIDE_LENGTH)), Objects::Get()->GetStructure());
				object->AddComponent(new Mesh());
				object->AddComponent(new LodBehaviour());
				object->AddComponent(new TerrainRender());
			}
		}
	}

	if (Waters::Get() != nullptr)
	{
		GameObject *object = new GameObject(Transform(Vector3(), Vector3()), Objects::Get()->GetStructure());
		object->AddComponent(new Mesh(new MeshWater()));
		object->AddComponent(new WaterRender());
	}

	// Runs the engine loop.
	const int exitCode = m_engine->Run();

	// Deletes the engine.
	delete configManager;
	delete m_engine;

	// Pauses the console.
	std::cin.get();
	return exitCode;
}
