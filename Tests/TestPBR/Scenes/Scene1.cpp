#include "Scene1.hpp"

#include <Inputs/Input.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Skyboxes/MaterialSkybox.hpp>
#include <Uis/Uis.hpp>
#include "CameraFree.hpp"

namespace test {
Scene1::Scene1() :
	Scene(std::make_unique<CameraFree>()),
	m_overlayDebug(&Uis::Get()->GetCanvas()) {
	Input::Get()->GetButton("captureMouse")->OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}
	}, this);
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
			sphere->AddComponent<Mesh>(ModelSphere::Create(1.0f, 30, 30));
			sphere->AddComponent<MaterialDefault>(Colour::Red, Image2d::Create("Objects/Testing/Diffuse.png"), j / 5.0f, i / 5.0f,
				nullptr, // Image2d::Create("Objects/Testing/Material.png")
				Image2d::Create("Objects/Testing/Normal.png"));
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();

			auto teapot = GetStructure()->CreateEntity();
			teapot->AddComponent<Transform>(Vector3f(i * 1.6f, j, 6.0f), Vector3f(), Vector3f(0.14f));
			teapot->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Tea.obj"));
			teapot->AddComponent<MaterialDefault>(Colour::White, nullptr, j / 5.0f, i / 5.0f);
			teapot->AddComponent<MeshRender>();
			teapot->AddComponent<ShadowRender>();
		}
	}

	/*auto dragon = GetStructure()->CreateEntity();
	dragon->AddComponent<Transform>(Vector3f(6.0f, 0.0f, 0.0f), Vector3f(0.0f, Maths::Radians(-90.0f), 0.0f), Vector3f(0.4f));
	dragon->AddComponent<Mesh>(ModelObj::Create("Objects/Testing/Model_Dragon.obj"));
	dragon->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.7f, 0.1f);
	dragon->AddComponent<MeshRender>();
	dragon->AddComponent<ShadowRender>();*/
}

void Scene1::Update() {
}

bool Scene1::IsPaused() const {
	return false;
}
}
