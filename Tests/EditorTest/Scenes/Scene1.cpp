#include "Scene1.hpp"

#include <Inputs/Input.hpp>
#include <Files/File.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Devices/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/DefaultMaterial.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SlideDriver.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Obj/ObjModel.hpp>
#include <Models/Shapes/CubeModel.hpp>
#include <Models/Shapes/CylinderModel.hpp>
#include <Models/Shapes/SphereModel.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Physics/Colliders/CapsuleCollider.hpp>
#include <Physics/Colliders/ConeCollider.hpp>
#include <Physics/Colliders/ConvexHullCollider.hpp>
#include <Physics/Colliders/CubeCollider.hpp>
#include <Physics/Colliders/CylinderCollider.hpp>
#include <Physics/Colliders/HeightfieldCollider.hpp>
#include <Physics/Colliders/SphereCollider.hpp>
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Uis/Uis.hpp>
#include <Files/Json/Json.hpp>
#include "FpsCamera.hpp"

namespace test {
static const Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(std::make_unique<FpsCamera>()),
	soundScreenshot("Sounds/Screenshot.ogg") {
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

	Input::Get()->GetButton("spawnSphere")->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			auto cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			auto cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GetStructure()->CreateEntity();
			sphere->AddComponent<Transform>(cameraPosition, Vector3f());
			sphere->AddComponent<Mesh>(SphereModel::Create(0.5f, 32, 32), 
				std::make_unique<DefaultMaterial>(Colour::White, nullptr, 0.0f, 1.0f));
			auto rigidbody = sphere->AddComponent<Rigidbody>(std::make_unique<SphereCollider>(), 0.5f);
			rigidbody->AddForce(std::make_unique<Force>(-3.0f * (Quaternion(cameraRotation) * Vector3f::Front).Normalize(), 2s));

			auto sphereLight = GetStructure()->CreateEntity();
			sphereLight->AddComponent<Transform>(Vector3f(0.0f, 0.7f, 0.0f))->SetParent(sphere);
			sphereLight->AddComponent<Light>(Colour::Aqua, 4.0f);
		}
	});
	Input::Get()->GetButton("captureMouse")->OnButton().connect(this, [this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}
	});
}

void Scene1::Start() {
	GetPhysics()->SetGravity({0.0f, -9.81f, 0.0f});
	GetPhysics()->SetAirDensity(1.0f);

	auto player = GetStructure()->CreateEntity("Objects/Player/Player.json");
	player->AddComponent<Transform>(Vector3f(0.0f, 2.0f, 0.0f), Vector3f(0.0f, Maths::Radians(180.0f), 0.0f));

	auto skybox = GetStructure()->CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json");
	skybox->AddComponent<Transform>(Vector3f(), Vector3f(), Vector3f(2048.0f));

	auto sun = GetStructure()->CreateEntity();
	sun->AddComponent<Transform>(Vector3f(1000.0f, 5000.0f, -4000.0f), Vector3f(), Vector3f(18.0f));
	//sun->AddComponent<CelestialBody>(CelestialBody::Type::Sun);
	sun->AddComponent<Light>(Colour::White);

	auto plane = GetStructure()->CreateEntity();
	plane->AddComponent<Transform>(Vector3f(0.0f, -0.5f, 0.0f), Vector3f(), Vector3f(50.0f, 1.0f, 50.0f));
	plane->AddComponent<Mesh>(CubeModel::Create(Vector3f(1.0f, 1.0f, 1.0f)), 
		std::make_unique<DefaultMaterial>(Colour::White, Image2d::Create("Undefined2.png", VK_FILTER_NEAREST)));
	plane->AddComponent<Rigidbody>(std::make_unique<CubeCollider>(Vector3f(1.0f, 1.0f, 1.0f)), 0.0f, 0.5f);

	static const std::vector cubeColours = {Colour::Red, Colour::Lime, Colour::Yellow, Colour::Blue, Colour::Purple, Colour::Grey, Colour::White};

	for (int32_t i = 0; i < 5; i++) {
		for (int32_t j = 0; j < 5; j++) {
			auto cube = GetStructure()->CreateEntity();
			cube->AddComponent<Transform>(Vector3f(static_cast<float>(i), static_cast<float>(j) + 0.5f, -10.0f));
			cube->AddComponent<Mesh>(CubeModel::Create(Vector3f(1.0f, 1.0f, 1.0f)), 
				std::make_unique<DefaultMaterial>(cubeColours[static_cast<uint32_t>(Maths::Random(0.0f, static_cast<float>(cubeColours.size())))], nullptr, 0.5f, 0.3f));
			cube->AddComponent<Rigidbody>(std::make_unique<CubeCollider>(), 0.5f, 0.3f);
		}
	}

	auto cone = GetStructure()->CreateEntity();
	cone->AddComponent<Transform>(Vector3f(-3.0f, 2.0f, 10.0f));
	cone->AddComponent<Mesh>(CylinderModel::Create(1.0f, 0.0f, 2.0f, 28, 2), 
		std::make_unique<DefaultMaterial>(Colour::Blue, nullptr, 0.0f, 1.0f));
	cone->AddComponent<Rigidbody>(std::make_unique<ConeCollider>(1.0f, 2.0f), 
		/*std::make_unique<SphereCollider>(1.0f, Transform({0.0f, 2.0f, 0.0f})),*/ 1.5f);

	auto cylinder = GetStructure()->CreateEntity();
	cylinder->AddComponent<Transform>(Vector3f(-8.0f, 3.0f, 10.0f), Vector3f(0.0f, 0.0f, Maths::Radians(90.0f)));
	cylinder->AddComponent<Mesh>(CylinderModel::Create(1.1f, 1.1f, 2.2f, 32, 2), 
		std::make_unique<DefaultMaterial>(Colour::Red, nullptr, 0.0f, 1.0f));
	cylinder->AddComponent<Rigidbody>(std::make_unique<CylinderCollider>(1.1f, 2.2f), 2.5f);

	auto smokeSystem = GetStructure()->CreateEntity("Objects/Smoke/Smoke.json");
	smokeSystem->AddComponent<Transform>(Vector3f(-15.0f, 4.0f, 12.0f));
	//smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg", Audio::Type::Music, true, true);
}

void Scene1::Update() {
}

bool Scene1::IsPaused() const {
	return !uiStartLogo.IsFinished();
}
}
