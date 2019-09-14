#include "Scene1.hpp"

#include <Audio/Sound.hpp>
//#include <Animations/MeshAnimated.hpp>
#include <Emitters/EmitterCircle.hpp>
#include <Files/File.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Devices/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Resources/Resources.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Gltf/ModelGltf.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Models/Shapes/ModelCylinder.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Physics/Colliders/ColliderCapsule.hpp>
#include <Physics/Colliders/ColliderCone.hpp>
#include <Physics/Colliders/ColliderConvexHull.hpp>
#include <Physics/Colliders/ColliderCube.hpp>
#include <Physics/Colliders/ColliderCylinder.hpp>
#include <Physics/Colliders/ColliderHeightfield.hpp>
#include <Physics/Colliders/ColliderSphere.hpp>
#include <Graphics/Graphics.hpp>
#include <Serialized/Json/Json.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include <Uis/Uis.hpp>
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "Behaviours/Rotate.hpp"
#include "Terrain/MaterialTerrain.hpp"
#include "Terrain/Terrain.hpp"
#include "CameraFps.hpp"

namespace test
{
static const Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene{std::make_unique<CameraFps>()},
	m_buttonSpawnSphere{std::make_unique<ButtonMouse>(MouseButton::Left)},
	m_buttonCaptureMouse{std::make_unique<ButtonKeyboard>(Key::Escape), std::make_unique<ButtonKeyboard>(Key::M)},
	m_buttonSave{Key::K},
	m_uiStartLogo{&Uis::Get()->GetCanvas()},
	m_overlayDebug{&Uis::Get()->GetCanvas()}
{
	m_buttonSpawnSphere.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			auto cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GetStructure()->CreateEntity();
			sphere->AddComponent<Transform>(cameraPosition, Vector3f{}, 1.0f);
			sphere->AddComponent<Mesh>(ModelSphere::Create(0.5f, 32, 32));
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			rigidbody->AddForce(std::make_unique<Force>(-3.0f * (Quaternion{cameraRotation} * Vector3f::Front).Normalize(), 2s));
			sphere->AddComponent<ColliderSphere>(); 
			//sphere->AddComponent<ColliderSphere>(0.5f, Transform(Vector3(0.0f, 1.0f, 0.0f)));
			sphere->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();
			sphere->AddComponent<HeightDespawn>(-75.0f); 

			auto sphereLight = GetStructure()->CreateEntity();
			sphereLight->AddComponent<Transform>(Vector3f{0.0f, 0.7f, 0.0f})->SetParent(sphere);
			sphereLight->AddComponent<Light>(Colour::Aqua, 4.0f);
		}
	});

	m_buttonCaptureMouse.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}
	});

	m_buttonSave.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Resources::Get()->GetThreadPool().Enqueue([this]()
			{
				File sceneFile{"Scene1.json", std::make_unique<Json>()};

				for (auto &entity : GetStructure()->QueryAll())
				{
					auto &entityNode{sceneFile.GetNode()->AddProperty()};

					if (!entity->GetName().empty())
					{
						entityNode["name"] = entity->GetName();
					}

					for (auto &component : entity->GetComponents())
					{
						auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

						if (componentName)
						{
							auto child = entityNode.AddProperty(*componentName, {});
							Scenes::Get()->GetComponentRegister().Encode(*componentName, child, component.get());
						}
					}
				}

				sceneFile.Write();
			});
		}
	});

	m_uiStartLogo.SetAlphaDriver(std::make_unique<DriverConstant<float>>(1.0f));
	m_overlayDebug.SetAlphaDriver(std::make_unique<DriverConstant<float>>(0.0f));

	m_uiStartLogo.OnFinished().Add([this]()
	{
		m_overlayDebug.SetAlphaDriver(std::make_unique<DriverSlide<float>>(0.0f, 1.0f, UI_SLIDE_TIME));
		Mouse::Get()->SetCursorHidden(true);
	});

	Mouse::Get()->OnDrop().Add([](std::vector<std::string> paths)
	{
		for (const auto &path : paths)
		{
			Log::Out("File dropped on window: ", path, '\n');
		}
	}, this);
	Window::Get()->OnMonitorConnect().Add([](Monitor *monitor, bool connected)
	{
		Log::Out("Monitor ", std::quoted(monitor->GetName()), " action: ", connected, '\n');
	}, this);
	Window::Get()->OnClose().Add([]()
	{
		Log::Out("Window has closed!\n");
	}, this);
	Window::Get()->OnIconify().Add([](bool iconified)
	{
		Log::Out("Iconified: ", iconified, '\n');
	}, this);
}

void Scene1::Start()
{
	GetPhysics()->SetGravity({0.0f, -9.81f, 0.0f});
	GetPhysics()->SetAirDensity(1.0f);

	auto player = GetStructure()->CreateEntity("Objects/Player/Player.json");
	player->AddComponent<Transform>(Vector3f{0.0f, 2.0f, 0.0f}, Vector3f{0.0f, Maths::Radians(180.0f), 0.0f});
	
	auto skybox = GetStructure()->CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json");
	skybox->AddComponent<Transform>(Vector3f{}, Vector3f{}, 2048.0f);

	//auto animated = GetStructure()->CreateEntity("Objects/Animated/Animated.json");
	//animated->AddComponent<Transform>(Vector3f{5.0f, 0.0f, 0.0f}, Vector3f{}, 0.3f);

	/*auto animated = GetStructure()->CreateEntity();
	animated->AddComponent<Transform>(Vector3f{5.0f, 0.0f, 0.0f}, Vector3f{}, 0.3f);
	animated->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
	animated->AddComponent<MaterialDefault>(Colour::White, Image2d::Create("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
	//animated->AddComponent<Rigidbody>(0.0f);
	//animated->AddComponent<ColliderCapsule>(3.0f, 6.0f, Transform(Vector3(0.0f, 2.5f, 0.0f)));
	animated->AddComponent<MeshRender>();
	animated->AddComponent<ShadowRender>();

	EntityPrefab prefabAnimated{"Prefabs/Animated.json"};
	prefabAnimated << *animated;
	prefabAnimated.Write();*/

	auto sun = GetStructure()->CreateEntity();
	sun->AddComponent<Transform>(Vector3f{1000.0f, 5000.0f, -4000.0f}, Vector3f{}, 18.0f);
	//sun->AddComponent<CelestialBody>(CelestialBody::Type::Sun);
	sun->AddComponent<Light>(Colour::White);

	auto plane = GetStructure()->CreateEntity();
	plane->AddComponent<Transform>(Vector3f{0.0f, -0.5f, 0.0f}, Vector3f{}, Vector3f{50.0f, 1.0f, 50.0f});
	plane->AddComponent<Mesh>(ModelCube::Create(Vector3f(1.0f, 1.0f, 1.0f)));
	plane->AddComponent<MaterialDefault>(Colour::White, Image2d::Create("Undefined2.png", VK_FILTER_NEAREST));
	plane->AddComponent<Rigidbody>(0.0f, 0.5f);
	plane->AddComponent<ColliderCube>(Vector3f{1.0f, 1.0f, 1.0f});
	plane->AddComponent<MeshRender>();
	plane->AddComponent<ShadowRender>();

	auto terrain = GetStructure()->CreateEntity();
	terrain->AddComponent<Transform>(Vector3f{0.0f, -10.0f, 0.0f});
	terrain->AddComponent<Mesh>(ModelCube::Create({50.0f, 1.0f, 50.0f}));
	//terrain->AddComponent<MaterialDefault>(Colour::White, Image2d::Create("Undefined2.png", VK_FILTER_NEAREST));
	terrain->AddComponent<MaterialTerrain>(Image2d::Create("Objects/Terrain/Grass.png"), Image2d::Create("Objects/Terrain/Rocks.png"));
	terrain->AddComponent<MeshRender>();
	terrain->AddComponent<ShadowRender>();

	/*auto terrain = GetStructure()->CreateEntity();
	terrain->AddComponent<Transform>();
	terrain->AddComponent<Mesh>();
	terrain->AddComponent<MaterialTerrain>(Image2d::Create("Objects/Terrain/Grass.png"), Image2d::Create("Objects/Terrain/Rocks.png"));
	terrain->AddComponent<Terrain>(150.0f, 2.0f);
	terrain->AddComponent<Rigidbody>(0.0f, 0.7f);
	terrain->AddComponent<ColliderHeightfield>();
	terrain->AddComponent<MeshRender>();
	terrain->AddComponent<ShadowRender>();*/

	EntityPrefab prefabTerrain{"Prefabs/Terrain.json"};
	prefabTerrain << *terrain;
	prefabTerrain.Write();

	static const std::vector cubeColours{Colour::Red, Colour::Lime, Colour::Yellow, Colour::Blue, Colour::Purple, Colour::Grey, Colour::White};

	for (int32_t i{}; i < 5; i++)
	{
		for (int32_t j{}; j < 5; j++)
		{
			auto cube = GetStructure()->CreateEntity();
			cube->AddComponent<Transform>(Vector3f{static_cast<float>(i), static_cast<float>(j) + 0.5f, -10.0f});
			cube->AddComponent<Mesh>(ModelCube::Create(Vector3f{1.0f, 1.0f, 1.0f}));
			cube->AddComponent<MaterialDefault>(cubeColours[static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(cubeColours.size())))], nullptr, 0.5f, 0.3f);
			cube->AddComponent<Rigidbody>(0.5f, 0.3f);
			cube->AddComponent<ColliderCube>();
			cube->AddComponent<MeshRender>();
			cube->AddComponent<ShadowRender>();
		}
	}

	auto suzanne = GetStructure()->CreateEntity();
	suzanne->AddComponent<Transform>(Vector3f{-1.0f, 2.0f, 10.0f});
	suzanne->AddComponent<Mesh>(ModelObj::Create("Objects/Suzanne/Suzanne.obj"));
	suzanne->AddComponent<MaterialDefault>(Colour::Red, nullptr, 0.2f, 0.8f);
	suzanne->AddComponent<MeshRender>();
	suzanne->AddComponent<ShadowRender>(); 
	
	//auto suzanne1 = GetStructure()->CreateEntity();
	//suzanne1->AddComponent<Transform>(Vector3f{-1.0f, 2.0f, 10.0f});
	//suzanne1->AddComponent<Mesh>(ModelGltf::Create("Objects/Suzanne/Suzanne.glb"));
	//suzanne1->AddComponent<MaterialDefault>(Colour::Red, nullptr, 0.5f, 0.2f);
	//suzanne1->AddComponent<MeshRender>();
	//suzanne1->AddComponent<ShadowRender>();

	auto teapot1 = GetStructure()->CreateEntity();
	teapot1->AddComponent<Transform>(Vector3f{4.0f, 2.0f, 10.0f}, Vector3f{}, 0.2f);
	teapot1->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Tea.obj"));
	teapot1->AddComponent<MaterialDefault>(Colour::Fuchsia, nullptr, 0.9f, 0.4f, nullptr, Image2d::Create("Objects/Testing/Normal.png"));
	//teapot1->AddComponent<Rigidbody>(1.0f);
	//teapot1->AddComponent<ColliderConvexHull>();
	teapot1->AddComponent<Rotate>(Maths::Radians(Vector3f(50.0f, 30.0f, 40.0f)), 0);
	teapot1->AddComponent<NameTag>("Vector3", 1.4f);
	teapot1->AddComponent<MeshRender>();
	teapot1->AddComponent<ShadowRender>();

	EntityPrefab prefabTeapot1{"Prefabs/Teapot1.json"};
	prefabTeapot1 << *teapot1;
	prefabTeapot1.Write();

	auto teapotCone = GetStructure()->CreateEntity();
	teapotCone->AddComponent<Transform>(Vector3f{0.0f, 10.0f, 0.0f}, Vector3f{}, 3.0f)->SetParent(teapot1);
	teapotCone->AddComponent<Mesh>(ModelCylinder::Create(1.0f, 0.0f, 2.0f, 24, 2));
	teapotCone->AddComponent<MaterialDefault>(Colour::Fuchsia, nullptr, 0.5f, 0.6f);
	teapotCone->AddComponent<MeshRender>();
	teapotCone->AddComponent<ShadowRender>();

	auto teapotConeLight = GetStructure()->CreateEntity();
	teapotConeLight->SetName("TeapotConeLight");
	teapotConeLight->AddComponent<Transform>(Vector3f{0.0f, 2.0f, 0.0f})->SetParent(teapotCone);
	teapotConeLight->AddComponent<Light>(Colour::Red, 6.0f);

	auto teapotConeSphere = GetStructure()->CreateEntity();
	teapotConeSphere->AddComponent<Transform>(Vector3f{0.0f, 1.5f, 0.0f}, Vector3f{}, 0.5f)->SetParent(teapotCone);
	teapotConeSphere->AddComponent<Mesh>(ModelSphere::Create(1.0f, 32, 32));
	teapotConeSphere->AddComponent<MaterialDefault>(Colour::Fuchsia, nullptr, 0.5f, 0.6f);
	teapotConeSphere->AddComponent<MeshRender>();
	teapotConeSphere->AddComponent<ShadowRender>();

	auto teapot2 = GetStructure()->CreateEntity();
	teapot2->AddComponent<Transform>(Vector3f{7.5f, 2.0f, 10.0f}, Vector3f{}, 0.2f);
	teapot2->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Tea.obj"));
	teapot2->AddComponent<MaterialDefault>(Colour::Lime, nullptr, 0.6f, 0.7f); 
	//teapot2->AddComponent<Rigidbody>(1.0f);
	//teapot2->AddComponent<ColliderConvexHull>();
	teapot2->AddComponent<Rotate>(Maths::Radians(Vector3f(50.0f, 30.0f, 40.0f)), 1);
	teapot2->AddComponent<NameTag>("Vector3->Quaternion->Vector3", 1.4f);
	teapot2->AddComponent<MeshRender>();
	teapot2->AddComponent<ShadowRender>();

	auto teapot3 = GetStructure()->CreateEntity();
	teapot3->AddComponent<Transform>(Vector3f{11.0f, 2.0f, 10.0f}, Vector3f{}, 0.2f);
	teapot3->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Tea.obj"));
	teapot3->AddComponent<MaterialDefault>(Colour::Teal, nullptr, 0.8f, 0.2f); 
	//teapot3->AddComponent<Rigidbody>(1.0f);
	//teapot3->AddComponent<ColliderConvexHull>();
	teapot3->AddComponent<Rotate>(Maths::Radians(Vector3f{50.0f, 30.0f, 40.0f}), 2);
	teapot3->AddComponent<NameTag>("Rigigbody Method\nVector3->btQuaternion->Vector3", 1.4f);
	teapot3->AddComponent<MeshRender>();
	teapot3->AddComponent<ShadowRender>();

	auto cone = GetStructure()->CreateEntity();
	cone->AddComponent<Transform>(Vector3f{-3.0f, 2.0f, 10.0f});
	cone->AddComponent<Mesh>(ModelCylinder::Create(1.0f, 0.0f, 2.0f, 28, 2));
	cone->AddComponent<MaterialDefault>(Colour::Blue, nullptr, 0.0f, 1.0f);
	cone->AddComponent<Rigidbody>(1.5f);
	cone->AddComponent<ColliderCone>(1.0f, 2.0f);
	cone->AddComponent<ColliderSphere>(1.0f, Transform{{0.0f, 2.0f, 0.0f}});
	cone->AddComponent<MeshRender>();
	cone->AddComponent<ShadowRender>();

	auto cylinder = GetStructure()->CreateEntity();
	cylinder->AddComponent<Transform>(Vector3f{-8.0f, 3.0f, 10.0f}, Vector3f{0.0f, 0.0f, Maths::Radians(90.0f)});
	cylinder->AddComponent<Mesh>(ModelCylinder::Create(1.1f, 1.1f, 2.2f, 32, 2));
	cylinder->AddComponent<MaterialDefault>(Colour::Red, nullptr, 0.0f, 1.0f);
	cylinder->AddComponent<Rigidbody>(2.5f);
	cylinder->AddComponent<ColliderCylinder>(1.1f, 2.2f);
	cylinder->AddComponent<MeshRender>();
	cylinder->AddComponent<ShadowRender>();

	auto smokeSystem = GetStructure()->CreateEntity("Objects/Smoke/Smoke.json");
	smokeSystem->AddComponent<Transform>(Vector3f{-15.0f, 4.0f, 12.0f});
	//smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg", Audio::Type::Music, true, true);

	EntityPrefab prefabSmokeSystem{"Prefabs/SmokeSystem.json"};
	prefabSmokeSystem << *smokeSystem;
	prefabSmokeSystem.Write();
}

void Scene1::Update()
{
	auto teapotConeLight = GetStructure()->GetEntity("TeapotConeLight");
	auto transform = teapotConeLight->GetComponent<Transform>();
}

bool Scene1::IsPaused() const
{
	return !m_uiStartLogo.IsFinished();
}
}
