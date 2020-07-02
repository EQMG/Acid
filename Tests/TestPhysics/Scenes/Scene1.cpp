#include "Scene1.hpp"

#include <Audio/Sound.hpp>
#include <Animations/AnimatedMesh.hpp>
#include <Files/File.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Devices/Mouse.hpp>
#include <Inputs/Inputs.hpp>
#include <Lights/Light.hpp>
#include <Resources/Resources.hpp>
#include <Materials/DefaultMaterial.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SlideDriver.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Shapes/CubeModel.hpp>
#include <Models/Shapes/CylinderModel.hpp>
#include <Models/Shapes/SphereModel.hpp>
#include <Particles/Emitters/CircleEmitter.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Physics/Colliders/CapsuleCollider.hpp>
#include <Physics/Colliders/ConeCollider.hpp>
#include <Physics/Colliders/ConvexHullCollider.hpp>
#include <Physics/Colliders/CubeCollider.hpp>
#include <Physics/Colliders/CylinderCollider.hpp>
#include <Physics/Colliders/HeightfieldCollider.hpp>
#include <Physics/Colliders/SphereCollider.hpp>
#include <Graphics/Graphics.hpp>
#include <Files/Json/Json.hpp>
#include <Files/File.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/SkyboxMaterial.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Uis/Uis.hpp>
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "Behaviours/Rotate.hpp"
#include "Terrain/TerrainMaterial.hpp"
#include "Terrain/Terrain.hpp"
#include "FpsCamera.hpp"

namespace test {
constexpr Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(std::make_unique<FpsCamera>()) {
	//uiStartLogo.SetTransform({UiMargins::All});
	uiStartLogo.SetAlphaDriver<ConstantDriver>(1.0f);
	uiStartLogo.OnFinished().connect(this, [this]() {
		overlayDebug.SetAlphaDriver<SlideDriver>(0.0f, 1.0f, UI_SLIDE_TIME);
		Mouse::Get()->SetCursorHidden(true);
	});
	Uis::Get()->GetCanvas().AddChild(&uiStartLogo);

	//overlayDebug.SetTransform({{100, 36}, UiAnchor::LeftBottom});
	overlayDebug.GetConstraints().SetWidth<PixelConstraint>(100)
		.SetHeight<PixelConstraint>(36)
		.SetX<PixelConstraint>(0, UiAnchor::Left)
		.SetY<PixelConstraint>(0, UiAnchor::Bottom);
	overlayDebug.SetAlphaDriver<ConstantDriver>(0.0f);
	Uis::Get()->GetCanvas().AddChild(&overlayDebug);

	Inputs::Get()->GetButton("spawnSphere")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			auto cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GetStructure()->CreateEntity();
			sphere->AddComponent<Transform>(cameraPosition, Vector3f());
			sphere->AddComponent<Mesh>(SphereModel::Create(0.5f, 32, 32), 
				std::make_unique<DefaultMaterial>(Colour::White, nullptr, 0.0f, 1.0f));
			auto rigidbody = sphere->AddComponent<Rigidbody>(std::make_unique<SphereCollider>(), 0.5f);
			rigidbody->AddForce(std::make_unique<Force>(-3.0f * (Quaternion(cameraRotation) * Vector3f::Front).Normalize(), 2s));
			//sphere->AddComponent<SphereCollider>(0.5f, Transform(Vector3(0.0f, 1.0f, 0.0f)));
			sphere->AddComponent<ShadowRender>();
			sphere->AddComponent<HeightDespawn>(-75.0f);

			auto sphereLight = GetStructure()->CreateEntity();
			sphereLight->AddComponent<Transform>(Vector3f(0.0f, 0.7f, 0.0f))->SetParent(sphere);
			sphereLight->AddComponent<Light>(Colour::Aqua, 4.0f);
		}
	});

	Inputs::Get()->GetButton("captureMouse")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}
	});

	Inputs::Get()->GetButton("save")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Resources::Get()->GetThreadPool().Enqueue([this]() {
				File sceneFile("Scene1.json", std::make_unique<Json>());

				auto entitiesNode = sceneFile.GetNode()["entities"];

				for (auto &entity : GetStructure()->QueryAll()) {
					auto &entityNode = entitiesNode->AddProperty();

					if (!entity->GetName().empty()) {
						entityNode["name"] = entity->GetName();
					}

					for (auto &component : entity->GetComponents()) {
						if (auto componentName = component->GetTypeName(); !componentName.empty()) {
							entityNode[componentName].Set(component);
						}
					}
				}

				sceneFile.Write(NodeFormat::Beautified);
			});
		}
	});

	Mouse::Get()->OnDrop().connect(this, [](std::vector<std::string> paths) {
		for (const auto &path : paths) {
			Log::Out("File dropped on window: ", path, '\n');
		}
	});
	Window::Get()->OnMonitorConnect().connect(this, [](Monitor *monitor, bool connected) {
		Log::Out("Monitor ", std::quoted(monitor->GetName()), " action: ", connected, '\n');
	});
	Window::Get()->OnClose().connect(this, []() {
		Log::Out("Window has closed!\n");
	});
	Window::Get()->OnIconify().connect(this, [](bool iconified) {
		Log::Out("Iconified: ", iconified, '\n');
	});
}

void Scene1::Start() {
	GetPhysics()->SetGravity({0.0f, -9.81f, 0.0f});
	GetPhysics()->SetAirDensity(1.0f);

	auto player = GetStructure()->CreateEntity("Objects/Player/Player.json");
	player->AddComponent<Transform>(Vector3f(0.0f, 2.0f, 0.0f), Vector3f(0.0f, Maths::Radians(180.0f), 0.0f));

	auto skybox = GetStructure()->CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json");
	skybox->AddComponent<Transform>(Vector3f(), Vector3f(), Vector3f(2048.0f));

	//auto animated = GetStructure()->CreateEntity("Objects/Animated/Animated.json");
	//animated->AddComponent<Transform>(Vector3f(5.0f, 0.0f, 0.0f), Vector3f(), Vector3f(0.3f));

	auto animated = GetStructure()->CreateEntity();
	animated->AddComponent<Transform>(Vector3f(5.0f, 0.0f, 0.0f), Vector3f(), Vector3f(0.3f));
	animated->AddComponent<AnimatedMesh>("Objects/Animated/Model.dae", 
		std::make_unique<DefaultMaterial>(Colour::White, Image2d::Create("Objects/Animated/Diffuse.png"), 0.7f, 0.6f));
	//animated->AddComponent<Rigidbody>(std::make_unique<CapsuleCollider>(3.0f, 6.0f, Transform(Vector3(0.0f, 2.5f, 0.0f))), 0.0f);
	animated->AddComponent<ShadowRender>();

#ifdef ACID_DEBUG
	EntityPrefab prefabAnimated("Prefabs/Animated.json");
	prefabAnimated << *animated;
	prefabAnimated.Write(NodeFormat::Beautified);
#endif

	auto sun = GetStructure()->CreateEntity();
	sun->AddComponent<Transform>(Vector3f(1000.0f, 5000.0f, -4000.0f), Vector3f(), Vector3f(18.0f));
	//sun->AddComponent<CelestialBody>(CelestialBody::Type::Sun);
	sun->AddComponent<Light>(Colour::White);

	auto plane = GetStructure()->CreateEntity();
	plane->AddComponent<Transform>(Vector3f(0.0f, -0.5f, 0.0f), Vector3f(), Vector3f(50.0f, 1.0f, 50.0f));
	plane->AddComponent<Mesh>(CubeModel::Create({1.0f, 1.0f, 1.0f}),
		std::make_unique<DefaultMaterial>(Colour::White, Image2d::Create("Undefined2.png", VK_FILTER_NEAREST)));
	plane->AddComponent<Rigidbody>(std::make_unique<CubeCollider>(Vector3f(1.0f, 1.0f, 1.0f)), 0.0f, 0.5f);
	plane->AddComponent<ShadowRender>();

	auto terrain = GetStructure()->CreateEntity();
	terrain->AddComponent<Transform>(Vector3f(0.0f, -10.0f, 0.0f));
	terrain->AddComponent<Mesh>(CubeModel::Create({50.0f, 1.0f, 50.0f}), 
		std::make_unique<TerrainMaterial>(Image2d::Create("Objects/Terrain/Grass.png"), Image2d::Create("Objects/Terrain/Rocks.png")));
	terrain->AddComponent<ShadowRender>();

	//auto terrain = GetStructure()->CreateEntity();
	//terrain->AddComponent<Transform>();
	//terrain->AddComponent<Mesh>(nullptr,
	//	std::make_unique<TerrainMaterial>(Image2d::Create("Objects/Terrain/Grass.png"), Image2d::Create("Objects/Terrain/Rocks.png")));
	//terrain->AddComponent<Terrain>(150.0f, 2.0f);
	//terrain->AddComponent<Rigidbody>(std::make_unique<HeightfieldCollider>(), 0.0f, 0.7f);
	//terrain->AddComponent<ShadowRender>();

#ifdef ACID_DEBUG
	EntityPrefab prefabTerrain("Prefabs/Terrain.json");
	prefabTerrain << *terrain;
	prefabTerrain.Write(NodeFormat::Beautified);
#endif

	static const std::vector cubeColours = {Colour::Red, Colour::Lime, Colour::Yellow, Colour::Blue, Colour::Purple, Colour::Grey, Colour::White};

	for (int32_t i = 0; i < 5; i++) {
		for (int32_t j = 0; j < 5; j++) {
			auto cube = GetStructure()->CreateEntity();
			cube->AddComponent<Transform>(Vector3f(static_cast<float>(i), static_cast<float>(j) + 0.5f, -10.0f));
			cube->AddComponent<Mesh>(CubeModel::Create({1.0f, 1.0f, 1.0f}), 
				std::make_unique<DefaultMaterial>(cubeColours[static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(cubeColours.size())))], nullptr, 0.5f, 0.3f));
			cube->AddComponent<Rigidbody>(std::make_unique<CubeCollider>(), 0.5f, 0.3f);
			cube->AddComponent<ShadowRender>();
		}
	}

	auto suzanne = GetStructure()->CreateEntity();
	suzanne->AddComponent<Transform>(Vector3f(-1.0f, 2.0f, 10.0f));
	suzanne->AddComponent<Mesh>(Model::Create("Objects/Suzanne/Suzanne.obj"),
		std::make_unique<DefaultMaterial>(Colour::Red, nullptr, 0.2f, 0.8f));
	suzanne->AddComponent<ShadowRender>();

	//auto suzanne1 = GetStructure()->CreateEntity();
	//suzanne1->AddComponent<Transform>(Vector3f(-1.0f, 2.0f, 10.0f));
	//suzanne1->AddComponent<Mesh>(GltfModel::Create("Objects/Suzanne/Suzanne.glb"),
	//	std::make_unique<DefaultMaterial>(Colour::Red, nullptr, 0.5f, 0.2f));
	//suzanne1->AddComponent<MeshRender>();
	//suzanne1->AddComponent<ShadowRender>();

	auto teapot1 = GetStructure()->CreateEntity();
	teapot1->AddComponent<Transform>(Vector3f(4.0f, 2.0f, 10.0f), Vector3f(), Vector3f(0.2f));
	teapot1->AddComponent<Mesh>(Model::Create("Objects/Testing/Model_Tea.obj"), 
		std::make_unique<DefaultMaterial>(Colour::Fuchsia, nullptr, 0.9f, 0.4f, nullptr, Image2d::Create("Objects/Testing/Normal.png")));
	//teapot1->AddComponent<Rigidbody>(std::make_unique<ConvexHullCollider>(), 1.0f);
	teapot1->AddComponent<Rotate>(Maths::Radians(Vector3f(50.0f, 30.0f, 40.0f)), 0);
	teapot1->AddComponent<NameTag>("Vector3", 1.4f);
	teapot1->AddComponent<ShadowRender>();

#ifdef ACID_DEBUG
	EntityPrefab prefabTeapot1("Prefabs/Teapot1.json");
	prefabTeapot1 << *teapot1;
	prefabTeapot1.Write(NodeFormat::Beautified);
#endif

	auto teapotCone = GetStructure()->CreateEntity();
	teapotCone->AddComponent<Transform>(Vector3f(0.0f, 10.0f, 0.0f), Vector3f(), Vector3f(3.0f))->SetParent(teapot1);
	teapotCone->AddComponent<Mesh>(CylinderModel::Create(1.0f, 0.0f, 2.0f, 24, 2), 
		std::make_unique<DefaultMaterial>(Colour::Fuchsia, nullptr, 0.5f, 0.6f));
	teapotCone->AddComponent<ShadowRender>();

	auto teapotConeLight = GetStructure()->CreateEntity();
	teapotConeLight->SetName("TeapotConeLight");
	teapotConeLight->AddComponent<Transform>(Vector3f(0.0f, 2.0f, 0.0f))->SetParent(teapotCone);
	teapotConeLight->AddComponent<Light>(Colour::Red, 6.0f);

	auto teapotConeSphere = GetStructure()->CreateEntity();
	teapotConeSphere->AddComponent<Transform>(Vector3f(0.0f, 1.5f, 0.0f), Vector3f(), Vector3f(0.5f))->SetParent(teapotCone);
	teapotConeSphere->AddComponent<Mesh>(SphereModel::Create(1.0f, 32, 32), 
		std::make_unique<DefaultMaterial>(Colour::Fuchsia, nullptr, 0.5f, 0.6f));
	teapotConeSphere->AddComponent<ShadowRender>();

	auto teapot2 = GetStructure()->CreateEntity();
	teapot2->AddComponent<Transform>(Vector3f(7.5f, 2.0f, 10.0f), Vector3f(), Vector3f(0.2f));
	teapot2->AddComponent<Mesh>(Model::Create("Objects/Testing/Model_Tea.obj"), 
		std::make_unique<DefaultMaterial>(Colour::Lime, nullptr, 0.6f, 0.7f));
	//teapot2->AddComponent<Rigidbody>(std::make_unique<ConvexHullCollider>(), 1.0f);
	teapot2->AddComponent<Rotate>(Maths::Radians(Vector3f(50.0f, 30.0f, 40.0f)), 1);
	teapot2->AddComponent<NameTag>("Vector3->Quaternion->Vector3", 1.4f);
	teapot2->AddComponent<ShadowRender>();

	auto teapot3 = GetStructure()->CreateEntity();
	teapot3->AddComponent<Transform>(Vector3f(11.0f, 2.0f, 10.0f), Vector3f(), Vector3f(0.2f));
	teapot3->AddComponent<Mesh>(Model::Create("Objects/Testing/Model_Tea.obj"), 
		std::make_unique<DefaultMaterial>(Colour::Teal, nullptr, 0.8f, 0.2f));
	//teapot3->AddComponent<Rigidbody>(std::make_unique<ConvexHullCollider>(), 1.0f);
	teapot3->AddComponent<Rotate>(Maths::Radians(Vector3f(50.0f, 30.0f, 40.0f)), 2);
	teapot3->AddComponent<NameTag>("Rigigbody Method\nVector3->btQuaternion->Vector3", 1.4f);
	teapot3->AddComponent<ShadowRender>();

	auto cone = GetStructure()->CreateEntity();
	cone->AddComponent<Transform>(Vector3f(-3.0f, 2.0f, 10.0f));
	cone->AddComponent<Mesh>(CylinderModel::Create(1.0f, 0.0f, 2.0f, 28, 2), 
		std::make_unique<DefaultMaterial>(Colour::Blue, nullptr, 0.0f, 1.0f));
	cone->AddComponent<Rigidbody>(std::make_unique<ConeCollider>(1.0f, 2.0f),
		/*std::make_unique<SphereCollider>(1.0f, Transform({0.0f, 2.0f, 0.0f})),*/ 1.5f);
	cone->AddComponent<ShadowRender>();

	auto cylinder = GetStructure()->CreateEntity();
	cylinder->AddComponent<Transform>(Vector3f(-8.0f, 3.0f, 10.0f), Vector3f(0.0f, 0.0f, Maths::Radians(90.0f)));
	cylinder->AddComponent<Mesh>(CylinderModel::Create(1.1f, 1.1f, 2.2f, 32, 2), 
		std::make_unique<DefaultMaterial>(Colour::Red, nullptr, 0.0f, 1.0f));
	cylinder->AddComponent<Rigidbody>(std::make_unique<CylinderCollider>(1.1f, 2.2f), 2.5f);
	cylinder->AddComponent<ShadowRender>();

	auto smokeSystem = GetStructure()->CreateEntity("Objects/Smoke/Smoke.json");
	smokeSystem->AddComponent<Transform>(Vector3f(-15.0f, 4.0f, 12.0f));
	//smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.wav", Audio::Type::Music, true, true);

#ifdef ACID_DEBUG
	EntityPrefab prefabSmokeSystem("Prefabs/SmokeSystem.json");
	prefabSmokeSystem << *smokeSystem;
	prefabSmokeSystem.Write(NodeFormat::Beautified);
#endif
}

void Scene1::Update() {
	auto teapotConeLight = GetStructure()->GetEntity("TeapotConeLight");
	auto transform = teapotConeLight->GetComponent<Transform>();
}

bool Scene1::IsPaused() const {
	return !uiStartLogo.IsFinished();
}
}
