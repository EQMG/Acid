#include "Scene1.hpp"

#include <Inputs/Inputs.hpp>
#include <Lights/Light.hpp>
#include <Materials/DefaultMaterial.hpp>
#include <Uis/Drivers/ConstantDriver.hpp>
#include <Uis/Drivers/SlideDriver.hpp>
#include <Meshes/Mesh.hpp>
#include <Models/Obj/ObjModel.hpp>
#include <Models/Shapes/CubeModel.hpp>
#include <Models/Shapes/SphereModel.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Skyboxes/SkyboxMaterial.hpp>
#include <Uis/Constraints/PixelConstraint.hpp>
#include <Uis/Constraints/RelativeConstraint.hpp>
#include <Uis/Uis.hpp>
#include "FreeCamera.hpp"

namespace test {
Scene1::Scene1() :
	Scene(std::make_unique<FreeCamera>()) {
	//overlayDebug.SetTransform({{100, 36}, UiAnchor::LeftBottom});
	overlayDebug.GetConstraints().SetWidth<PixelConstraint>(100)
		.SetHeight<PixelConstraint>(36)
		.SetX<PixelConstraint>(0, UiAnchor::Left)
		.SetY<PixelConstraint>(0, UiAnchor::Bottom);
	Uis::Get()->GetCanvas().AddChild(&overlayDebug);
	
	Inputs::Get()->GetButton("captureMouse")->OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}
	});
}

void Scene1::Start() {
	GetPhysics()->SetGravity({0.0f, -9.81f, 0.0f});
	GetPhysics()->SetAirDensity(1.0f);

	auto skybox = GetStructure()->CreateEntity("Objects/SkyboxSnowy/SkyboxSnowy.json");
	skybox->AddComponent<Transform>(Vector3f(), Vector3f(), Vector3f(1024.0f));

	auto sun = GetStructure()->CreateEntity();
	sun->AddComponent<Transform>(Vector3f(1000.0f, 5000.0f, -4000.0f), Vector3f(), Vector3f(18.0f));
	sun->AddComponent<Light>(Colour::White);

	for (uint32_t i = 0; i < 6; i++) {
		for (uint32_t j = 0; j < 6; j++) {
			auto sphere = GetStructure()->CreateEntity();
			sphere->AddComponent<Transform>(Vector3f(i, j, -6.0f), Vector3f(), Vector3f(0.5f));
			sphere->AddComponent<Mesh>(SphereModel::Create(1.0f, 30, 30), 
				std::make_unique<DefaultMaterial>(Colour::Red, Image2d::Create("Objects/Testing/Diffuse.png"), j / 5.0f, i / 5.0f,
				nullptr, // Image2d::Create("Objects/Testing/Material.png")
				Image2d::Create("Objects/Testing/Normal.png")));

			auto teapot = GetStructure()->CreateEntity();
			teapot->AddComponent<Transform>(Vector3f(i * 1.6f, j, 6.0f), Vector3f(), Vector3f(0.14f));
			teapot->AddComponent<Mesh>(ObjModel::Create("Objects/Testing/Model_Tea.obj"), 
				std::make_unique<DefaultMaterial>(Colour::White, nullptr, j / 5.0f, i / 5.0f));
		}
	}

	//auto dragon = GetStructure()->CreateEntity();
	//dragon->AddComponent<Transform>(Vector3f(6.0f, 0.0f, 0.0f), Vector3f(0.0f, Maths::Radians(-90.0f), 0.0f), Vector3f(0.4f));
	//dragon->AddComponent<Mesh>(ObjModel::Create("Objects/Testing/Model_Dragon.obj"),
	//	std::make_unique<DefaultMaterial>(Colour::White, nullptr, 0.7f, 0.1f));
}

void Scene1::Update() {
}

bool Scene1::IsPaused() const {
	return false;
}
}
