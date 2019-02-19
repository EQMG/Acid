#include "Scene1.hpp"

#include <thread>
#include <Animations/MeshAnimated.hpp>
#include <Emitters/EmitterCircle.hpp>
#include <Files/File.hpp>
#include <Gizmos/Gizmos.hpp>
#include <Files/FileSystem.hpp>
#include <Devices/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
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
#include <Renderer/Renderer.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Scenes/Scenes.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Uis/Uis.hpp>
#include <Serialized/Json/Json.hpp>
#include <Serialized/Xml/Xml.hpp>
#include <Serialized/Yaml/Yaml.hpp>
#include "CameraFps.hpp"

namespace test
{
	static const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

	Scene1::Scene1() :
		Scene(new CameraFps()),
		m_buttonSpawnSphere(ButtonMouse(MouseButton::Left)),
		m_buttonFullscreen(ButtonKeyboard(Key::F11)),
		m_buttonCaptureMouse(ButtonCompound::Create<ButtonKeyboard>(Key::Escape, Key::M)),
		m_buttonScreenshot(ButtonKeyboard(Key::F12)),
		m_buttonSave(ButtonKeyboard(Key::K)),
		m_buttonExit(ButtonKeyboard(Key::Delete)),
		m_soundScreenshot(Sound("Sounds/Screenshot.ogg")),
		m_uiStartLogo(std::make_unique<UiStartLogo>(Uis::Get()->GetContainer())),
		m_overlayDebug(std::make_unique<OverlayDebug>(Uis::Get()->GetContainer()))
	{
		m_uiStartLogo->SetAlphaDriver<DriverConstant<float>>(1.0f);
		m_overlayDebug->SetAlphaDriver<DriverConstant<float>>(0.0f);
	}

	void Scene1::Start()
	{
		GetPhysics()->SetGravity(Vector3(0.0f, -9.81f, 0.0f));
		GetPhysics()->SetAirDensity(1.0f);

		// Player.
		auto playerObject = GetStructure()->CreateEntity("Objects/Player/Player.xml", Transform(Vector3(0.0f, 2.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		auto skyboxObject = GetStructure()->CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated model.
#if !defined(ACID_BUILD_MSVC)
		auto animatedObject = GetStructure()->CreateEntity(Transform(Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), 0.3f));
		animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
		animatedObject->AddComponent<MaterialDefault>(Colour::White, Texture::Create("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
		animatedObject->AddComponent<Rigidbody>(0.0f);
		animatedObject->AddComponent<ColliderCapsule>(3.0f, 6.0f, Transform(Vector3(0.0f, 2.5f, 0.0f)));
		animatedObject->AddComponent<MeshRender>();
		animatedObject->AddComponent<ShadowRender>();
#endif

		// Entities.
		auto sun = GetStructure()->CreateEntity(Transform(Vector3(1000.0f, 5000.0f, -4000.0f), Vector3(), 18.0f));
		//	sun->AddComponent<CelestialBody>(CELESTIAL_SUN);
		sun->AddComponent<Light>(Colour::White);

		auto plane = GetStructure()->CreateEntity(Transform(Vector3(0.0f, -0.5f, 0.0f), Vector3(), Vector3(50.0f, 1.0f, 50.0f)));
		plane->AddComponent<Mesh>(ModelCube::Create(1.0f, 1.0f, 1.0f));
		plane->AddComponent<MaterialDefault>(Colour::Grey, Texture::Create("Undefined2.png"), 0.0f, 1.0f);
		plane->AddComponent<Rigidbody>(0.0f, 0.5f);
		plane->AddComponent<ColliderCube>(Vector3(1.0f, 1.0f, 1.0f));
		plane->AddComponent<MeshRender>();
		plane->AddComponent<ShadowRender>();

		static const std::vector cubeColours = {Colour::Red, Colour::Lime, Colour::Yellow, Colour::Blue, Colour::Purple, Colour::Grey, Colour::White};

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto cube = GetStructure()->CreateEntity(Transform(Vector3(i, j + 0.5f, -10.0f), Vector3(), 1.0f));
				cube->AddComponent<Mesh>(ModelCube::Create(1.0f, 1.0f, 1.0f));
				cube->AddComponent<MaterialDefault>(cubeColours[static_cast<uint32_t>(Maths::Random(0, cubeColours.size()))]);
				cube->AddComponent<Rigidbody>(0.5f, 0.3f);
				cube->AddComponent<ColliderCube>();
				cube->AddComponent<MeshRender>();
				cube->AddComponent<ShadowRender>();
			}
		}

		auto cone = GetStructure()->CreateEntity(Transform(Vector3(-3.0f, 2.0f, 10.0f), Vector3(), 1.0f));
		cone->AddComponent<Mesh>(ModelCylinder::Create(1.0f, 0.0f, 2.0f, 16, 8));
		cone->AddComponent<MaterialDefault>(Colour::Blue, nullptr, 0.0f, 1.0f);
		cone->AddComponent<Rigidbody>(1.5f);
		cone->AddComponent<ColliderCone>(1.0f, 2.0f);
		cone->AddComponent<ColliderSphere>(1.0f, Transform(Vector3(0.0f, 2.0f, 0.0f)));
		cone->AddComponent<MeshRender>();
		cone->AddComponent<ShadowRender>();

		auto cylinder = GetStructure()->CreateEntity(Transform(Vector3(-8.0f, 3.0f, 10.0f), Vector3(0.0f, 0.0f, 90.0f), 1.0f));
		cylinder->AddComponent<Mesh>(ModelCylinder::Create(1.1f, 1.1f, 2.2f, 16, 8));
		cylinder->AddComponent<MaterialDefault>(Colour::Red, nullptr, 0.0f, 1.0f);
		cylinder->AddComponent<Rigidbody>(2.5f);
		cylinder->AddComponent<ColliderCylinder>(1.1f, 2.2f);
		cylinder->AddComponent<MeshRender>();
		cylinder->AddComponent<ShadowRender>();

		auto smokeSystem = GetStructure()->CreateEntity("Objects/Smoke/Smoke.json", Transform(Vector3(-15.0f, 4.0f, 12.0f)));
		//	smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg", Transform::IDENTITY, SOUND_TYPE_MUSIC, true, true);
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere.WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GetStructure()->CreateEntity(Transform(cameraPosition, Vector3(), 1.0f));
			sphere->AddComponent<Mesh>(ModelSphere::Create(0.5f, 30, 30));
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			rigidbody->AddForce<Force>(-(cameraRotation.ToQuaternion() * Vector3::Front).Normalize() * 3.0f, Time::Seconds(2.0f));
			sphere->AddComponent<ColliderSphere>();
			//	sphere->AddComponent<ColliderSphere>(0.5f, Transform(Vector3(0.0f, 1.0f, 0.0f)));
			sphere->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<Light>(Colour::Aqua, 4.0f, Transform(Vector3(0.0f, 0.7f, 0.0f)));
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();

			//	auto gizmoType1 = GizmoType::Create(Model::Create("Gizmos/Arrow.obj"), 3.0f);
			//	Gizmos::Get()->AddGizmo(new Gizmo(gizmoType1, Transform(cameraPosition, cameraRotation), Colour::PURPLE));

			//	auto collisionObject = sphere->GetComponent<CollisionObject>();
			//	collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){
			//		Log::Out("Sphere_Undefined collided with '%s'\n", other->GetParent()->GetName().c_str());});
			//	collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){
			//		Log::Out("Sphere_Undefined seperated with '%s'\n", other->GetParent()->GetName().c_str());});
		}

		if (m_buttonFullscreen.WasDown())
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse->WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorHidden());
		}

		if (m_buttonScreenshot.WasDown())
		{
			m_soundScreenshot.Play();

			// TODO: Threading.
			std::thread t([]()
			{
				std::string filename = "Screenshots/" + Engine::GetDateTime() + ".png";
				Renderer::Get()->CaptureScreenshot(filename);
			});
			t.detach();
		}

		if (m_buttonSave.WasDown())
		{
			// TODO: Threading.
			std::thread t([this]()
			{
				auto sceneFile = File("Scene1.yaml", new Yaml());
				auto sceneNode = sceneFile.GetMetadata()->AddChild(new Metadata("Scene"));

				for (auto &entity : GetStructure()->QueryAll())
				{
					auto entityNode = sceneNode->AddChild(new Metadata());
					entityNode->AddChild(new Metadata("Name", "\"" + entity->GetName() + "\""));
					auto transformNode = entityNode->AddChild(new Metadata("Transform"));
					auto componentsNode = entityNode->AddChild(new Metadata("Components"));
					entity->GetLocalTransform().Encode(*transformNode);

					for (auto &component : entity->GetComponents())
					{
						//	if (component->IsFromPrefab())
						//	{
						//		continue;
						//	}

						auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

						if (componentName)
						{
							auto child = componentsNode->AddChild(new Metadata(*componentName));
							component->Encode(*child);
						}
					}
				}

				sceneFile.Write();
			});
			t.detach();
		}

		if (m_buttonExit.WasDown())
		{
			Engine::Get()->RequestClose(false);
		}

		if (m_uiStartLogo != nullptr && !m_uiStartLogo->IsStarting())
		{
			m_overlayDebug->SetAlphaDriver<DriverSlide<float>>(0.0f, 1.0f, UI_SLIDE_TIME);
			m_uiStartLogo = nullptr;
			Mouse::Get()->SetCursorHidden(true);
		}
	}

	bool Scene1::IsPaused() const
	{
		return m_uiStartLogo != nullptr;
	}
}
