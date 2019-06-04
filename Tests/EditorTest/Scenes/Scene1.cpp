#include "Scene1.hpp"

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
#include <Graphics/Graphics.hpp>
#include <Resources/Resources.hpp>
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
static const Time UI_SLIDE_TIME = 0.2s;

Scene1::Scene1() :
	Scene(new CameraFps()),
	m_buttonSpawnSphere(MouseButton::Left),
	m_buttonCaptureMouse(ButtonCompound::Create<ButtonKeyboard>(false, Key::Escape, Key::M)),
	m_buttonSave(Key::K),
	m_soundScreenshot("Sounds/Screenshot.ogg"),
	m_uiStartLogo(&Uis::Get()->GetCanvas()),
	m_overlayDebug(&Uis::Get()->GetCanvas())
{
	m_buttonSpawnSphere.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			Vector3f cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3f cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = GetStructure()->CreateEntity(Transform(cameraPosition, Vector3f(), 1.0f));
			sphere->AddComponent<Mesh>(ModelSphere::Create(0.5f, 30, 30));
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			rigidbody->AddForce<Force>(-3.0f * (Quaternion(cameraRotation) * Vector3f::Front).Normalize(), 2s);
			sphere->AddComponent<ColliderSphere>();
			//sphere->AddComponent<ColliderSphere>(0.5f, Transform(Vector3(0.0f, 1.0f, 0.0f)));
			sphere->AddComponent<MaterialDefault>(Colour::White, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();

			auto sphereLight = GetStructure()->CreateEntity(Transform(Vector3f(0.0f, 0.7f, 0.0f)));
			sphereLight->SetParent(sphere);
			sphereLight->AddComponent<Light>(Colour::Aqua, 4.0f);

			//auto gizmoType1 = GizmoType::Create(Model::Create("Gizmos/Arrow.obj"), 3.0f);
			//Gizmos::Get()->AddGizmo(new Gizmo(gizmoType1, Transform(cameraPosition, cameraRotation), Colour::PURPLE));

			//auto collisionObject = sphere->GetComponent<CollisionObject>();
			//collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){ Log::Out("Sphere_Undefined collided with '%s'\n", other->GetParent()->GetName().c_str());});
			//collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){ Log::Out("Sphere_Undefined seperated with '%s'\n", other->GetParent()->GetName().c_str());});
		}
	});

	m_buttonCaptureMouse->OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
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
				auto sceneFile = File("Scene1.yaml", std::make_unique<Yaml>());
				auto sceneNode = sceneFile.GetMetadata()->AddChild(new Metadata("Scene"));

				for (auto &entity : GetStructure()->QueryAll())
				{
					auto entityNode = sceneNode->AddChild(new Metadata());
					entityNode->AddChild(new Metadata("Name", "\"" + entity->GetName() + "\""));
					auto transformNode = entityNode->AddChild(new Metadata("Transform"));
					auto componentsNode = entityNode->AddChild(new Metadata("Components"));
					*transformNode << entity->GetLocalTransform();

					for (auto &component : entity->GetComponents())
					{
						//if (component->IsFromPrefab())
						//{
						//	continue;
						//}

						auto componentName = Scenes::Get()->GetComponentRegister().FindName(component.get());

						if (componentName)
						{
							auto child = componentsNode->AddChild(new Metadata(*componentName));
							Scenes::Get()->GetComponentRegister().Encode(*componentName, *child, component.get());
						}
					}
				}

				sceneFile.Write();
			});
		}
	});

	m_uiStartLogo.SetAlphaDriver(new DriverConstant<float>(1.0f));
	m_overlayDebug.SetAlphaDriver(new DriverConstant<float>(0.0f));

	m_uiStartLogo.OnFinished().Add([this]()
	{
		m_overlayDebug.SetAlphaDriver(new DriverSlide<float>(0.0f, 1.0f, UI_SLIDE_TIME));
		Mouse::Get()->SetCursorHidden(true);
	});
}

void Scene1::Start()
{
	GetPhysics()->SetGravity(Vector3f(0.0f, -9.81f, 0.0f));
	GetPhysics()->SetAirDensity(1.0f);

	// Player.
	auto playerObject = GetStructure()->CreateEntity("Objects/Player/Player.xml", Transform(Vector3f(0.0f, 2.0f, 0.0f), Vector3f(0.0f, Maths::Radians(180.0f), 0.0f)));

	// Skybox.
	auto skyboxObject = GetStructure()->CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3f(), Vector3f(), 2048.0f));

	// Entities.
	auto sun = GetStructure()->CreateEntity(Transform(Vector3f(1000.0f, 5000.0f, -4000.0f), Vector3f(), 18.0f));
	//sun->AddComponent<CelestialBody>(CELESTIAL_SUN);
	sun->AddComponent<Light>(Colour::White);

	auto plane = GetStructure()->CreateEntity(Transform(Vector3f(0.0f, -0.5f, 0.0f), Vector3f(), Vector3f(50.0f, 1.0f, 50.0f)));
	plane->AddComponent<Mesh>(ModelCube::Create(Vector3f(1.0f, 1.0f, 1.0f)));
	plane->AddComponent<MaterialDefault>(Colour::Grey, Image2d::Create("Undefined2.png", VK_FILTER_NEAREST), 0.0f, 1.0f);
	plane->AddComponent<Rigidbody>(0.0f, 0.5f);
	plane->AddComponent<ColliderCube>(Vector3f(1.0f, 1.0f, 1.0f));
	plane->AddComponent<MeshRender>();
	plane->AddComponent<ShadowRender>();

	static const std::vector cubeColours{ Colour::Red, Colour::Lime, Colour::Yellow, Colour::Blue, Colour::Purple, Colour::Grey, Colour::White };

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			auto cube = GetStructure()->CreateEntity(Transform(Vector3f(i, j + 0.5f, -10.0f), Vector3f(), 1.0f));
			cube->AddComponent<Mesh>(ModelCube::Create(Vector3f(1.0f, 1.0f, 1.0f)));
			cube->AddComponent<MaterialDefault>(cubeColours[static_cast<uint32_t>(Maths::Random(0, cubeColours.size()))]);
			cube->AddComponent<Rigidbody>(0.5f, 0.3f);
			cube->AddComponent<ColliderCube>();
			cube->AddComponent<MeshRender>();
			cube->AddComponent<ShadowRender>();
		}
	}

	auto cone = GetStructure()->CreateEntity(Transform(Vector3f(-3.0f, 2.0f, 10.0f), Vector3f(), 1.0f));
	cone->AddComponent<Mesh>(ModelCylinder::Create(1.0f, 0.0f, 2.0f, 16, 8));
	cone->AddComponent<MaterialDefault>(Colour::Blue, nullptr, 0.0f, 1.0f);
	cone->AddComponent<Rigidbody>(1.5f);
	cone->AddComponent<ColliderCone>(1.0f, 2.0f);
	cone->AddComponent<ColliderSphere>(1.0f, Transform(Vector3f(0.0f, 2.0f, 0.0f)));
	cone->AddComponent<MeshRender>();
	cone->AddComponent<ShadowRender>();

	auto cylinder = GetStructure()->CreateEntity(Transform(Vector3f(-8.0f, 3.0f, 10.0f), Vector3f(0.0f, 0.0f, Maths::Radians(90.0f)), 1.0f));
	cylinder->AddComponent<Mesh>(ModelCylinder::Create(1.1f, 1.1f, 2.2f, 16, 8));
	cylinder->AddComponent<MaterialDefault>(Colour::Red, nullptr, 0.0f, 1.0f);
	cylinder->AddComponent<Rigidbody>(2.5f);
	cylinder->AddComponent<ColliderCylinder>(1.1f, 2.2f);
	cylinder->AddComponent<MeshRender>();
	cylinder->AddComponent<ShadowRender>();

	auto smokeSystem = GetStructure()->CreateEntity("Objects/Smoke/Smoke.json", Transform(Vector3f(-15.0f, 4.0f, 12.0f)));
	//smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg, Audio::Type::Music, true, true);
}

void Scene1::Update()
{
}

bool Scene1::IsPaused() const
{
	return !m_uiStartLogo.IsFinished();
}
}
