#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Files/Json/FileJson.hpp>
#include <Gizmos/Gizmos.hpp>
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
#include <Models/Shapes/ModelCylinder.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Particles/Emitters/EmitterCircle.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Physics/Colliders/ColliderCapsule.hpp>
#include <Physics/Colliders/ColliderCone.hpp>
#include <Physics/Colliders/ColliderConvexHull.hpp>
#include <Physics/Colliders/ColliderCube.hpp>
#include <Physics/Colliders/ColliderCylinder.hpp>
#include <Physics/Colliders/ColliderHeightfield.hpp>
#include <Physics/Colliders/ColliderSphere.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>
#include <Scenes/EntityPrefab.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Terrain/MaterialTerrain.hpp>
#include <Terrain/Terrain.hpp>
#include <Uis/Uis.hpp>
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "Behaviours/Rotate.hpp"
#include "Skybox/CelestialBody.hpp"
#include "CameraFps.hpp"

namespace test
{
	static const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

	Scene1::Scene1() :
		Scene(new CameraFps(), new SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_buttonSpawnSphere(ButtonMouse({MOUSE_BUTTON_1})),
		m_buttonFullscreen(ButtonKeyboard({KEY_F11})),
		m_buttonCaptureMouse(ButtonKeyboard({KEY_M, KEY_ESCAPE})),
		m_buttonScreenshot(ButtonKeyboard({KEY_F12})),
		m_buttonSave(ButtonKeyboard({KEY_K})),
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
		auto playerObject = CreateEntity("Objects/Player/Player.xml", Transform(Vector3(0.0f, 2.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		auto skyboxObject = CreateEntity("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated.
		auto animatedObject = CreateEntity(Transform(Vector3(5.0f, 0.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f), 0.3f));
		animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
		animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
		animatedObject->AddComponent<Rigidbody>(0.0f);
		animatedObject->AddComponent<ColliderCapsule>(3.0f, 6.0f, Transform(Vector3(0.0f, 2.5f, 0.0f)));
		animatedObject->AddComponent<MeshRender>();
		animatedObject->AddComponent<ShadowRender>();

		// Entities.
		auto sun = CreateEntity(Transform(Vector3(1000.0f, 5000.0f, -4000.0f), Vector3(), 18.0f));
		sun->AddComponent<CelestialBody>(CELESTIAL_SUN);
		sun->AddComponent<Light>(Colour::WHITE);

		auto plane = CreateEntity(Transform(Vector3(0.0f, -0.5f, 0.0f), Vector3(), Vector3(50.0f, 1.0f, 50.0f)));
		plane->AddComponent<Mesh>(ModelCube::Resource(1.0f, 1.0f, 1.0f));
		plane->AddComponent<MaterialDefault>(Colour::GREY, Texture::Resource("Undefined2.png"), 0.0f, 1.0f);
		plane->AddComponent<Rigidbody>(0.0f, 0.5f);
		plane->AddComponent<ColliderCube>(Vector3(1.0f, 1.0f, 1.0f));
		plane->AddComponent<MeshRender>();
		plane->AddComponent<ShadowRender>();

		EntityPrefab prefabPlane = EntityPrefab("Plane.xml");
		prefabPlane.Write(*plane);
		prefabPlane.Save();

		auto terrain = CreateEntity(Transform());
		terrain->AddComponent<Mesh>();
		terrain->AddComponent<MaterialTerrain>(Texture::Resource("Objects/Terrain/Grass.png"), Texture::Resource("Objects/Terrain/Rocks.png"));
		terrain->AddComponent<Terrain>(150.0f, 2.0f);
		terrain->AddComponent<Rigidbody>(0.0f, 0.7f);
		terrain->AddComponent<ColliderHeightfield>();
		terrain->AddComponent<MeshRender>();
		terrain->AddComponent<ShadowRender>();

		static const std::vector cubeColours = {Colour::RED, Colour::LIME, Colour::YELLOW, Colour::BLUE, Colour::PURPLE, Colour::GREY, Colour::WHITE};

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto cube = CreateEntity(Transform(Vector3(i, j + 0.5f, -10.0f), Vector3(), 1.0f));
				cube->AddComponent<Mesh>(ModelCube::Resource(1.0f, 1.0f, 1.0f));
				cube->AddComponent<MaterialDefault>(cubeColours[static_cast<uint32_t>(Maths::Random(0, cubeColours.size()))]);
				cube->AddComponent<Rigidbody>(0.8f, 0.3f);
				cube->AddComponent<ColliderCube>();
				cube->AddComponent<MeshRender>();
				cube->AddComponent<ShadowRender>();
			}
		}

		auto teapot = CreateEntity(Transform(Vector3(4.0f, 2.0f, 10.0f), Vector3(), 0.2f));
		teapot->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		teapot->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
	//	teapot->AddComponent<Rigidbody>(1.0f);
	//	teapot->AddComponent<ColliderConvexHull>();
		teapot->AddComponent<Rotate>(Vector3(50.0f, 30.0f, 40.0f), 0);
		teapot->AddComponent<NameTag>("Vector3", 1.4f);
		teapot->AddComponent<MeshRender>();
		teapot->AddComponent<ShadowRender>();
		auto teapotCone = CreateEntity(Transform(Vector3(0.0f, 10.0f, 0.0f), Vector3(), 3.0f));
		teapotCone->SetParent(teapot);
		teapotCone->AddComponent<Mesh>(ModelCylinder::Resource(1.0f, 0.0f, 2.0f, 16, 8));
		teapotCone->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
		teapotCone->AddComponent<Light>(Colour::WHITE, 6.0f, Transform(Vector3(0.0f, 0.5f, 0.0f)));
		teapotCone->AddComponent<MeshRender>();
		teapotCone->AddComponent<ShadowRender>();

		auto teapot2 = CreateEntity(Transform(Vector3(7.5f, 2.0f, 10.0f), Vector3(), 0.2f));
		teapot2->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		teapot2->AddComponent<MaterialDefault>(Colour::LIME, nullptr, 0.0f, 1.0f);
	//	teapot2->AddComponent<Rigidbody>(1.0f);
	//	teapot2->AddComponent<ColliderConvexHull>();
		teapot2->AddComponent<Rotate>(Vector3(50.0f, 30.0f, 40.0f), 1);
		teapot2->AddComponent<NameTag>("Vector3->Quaternion->Vector3", 1.4f);
		teapot2->AddComponent<MeshRender>();
		teapot2->AddComponent<ShadowRender>();

		auto teapot3 = CreateEntity(Transform(Vector3(11.0f, 2.0f, 10.0f), Vector3(), 0.2f));
		teapot3->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
		teapot3->AddComponent<MaterialDefault>(Colour::TEAL, nullptr, 0.0f, 1.0f);
	//	teapot3->AddComponent<Rigidbody>(1.0f);
	//	teapot3->AddComponent<ColliderConvexHull>();
		teapot3->AddComponent<Rotate>(Vector3(50.0f, 30.0f, 40.0f), 2);
		teapot3->AddComponent<NameTag>("Rigigbody Method\nVector3->btQuaternion->Vector3", 1.4f);
		teapot3->AddComponent<MeshRender>();
		teapot3->AddComponent<ShadowRender>();

		auto cone = CreateEntity(Transform(Vector3(-3.0f, 2.0f, 10.0f), Vector3(), 1.0f));
		cone->AddComponent<Mesh>(ModelCylinder::Resource(1.0f, 0.0f, 2.0f, 16, 8));
		cone->AddComponent<MaterialDefault>(Colour::BLUE, nullptr, 0.0f, 1.0f);
		cone->AddComponent<Rigidbody>(1.5f);
		cone->AddComponent<ColliderCone>(1.0f, 2.0f);
		cone->AddComponent<ColliderSphere>(1.0f, Transform(Vector3(0.0f, 2.0f, 0.0f)));
		cone->AddComponent<MeshRender>();
		cone->AddComponent<ShadowRender>();

		auto cylinder = CreateEntity(Transform(Vector3(-8.0f, 3.0f, 10.0f), Vector3(0.0f, 0.0f, 90.0f), 1.0f));
		cylinder->AddComponent<Mesh>(ModelCylinder::Resource(1.1f, 1.1f, 2.2f, 16, 8));
		cylinder->AddComponent<MaterialDefault>(Colour::RED, nullptr, 0.0f, 1.0f);
		cylinder->AddComponent<Rigidbody>(2.5f);
		cylinder->AddComponent<ColliderCylinder>(1.1f, 2.2f);
		cylinder->AddComponent<MeshRender>();
		cylinder->AddComponent<ShadowRender>();

		auto smokeSystem = CreateEntity("Objects/Smoke/Smoke.json", Transform(Vector3(-15.0f, 4.0f, 12.0f)));
	//	smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg", Transform::IDENTITY, SOUND_TYPE_MUSIC, true, true);

		EntityPrefab prefabSmokeSystem = EntityPrefab("SmokeSystem.json");
		prefabSmokeSystem.Write(*smokeSystem);
		prefabSmokeSystem.Save();
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere.WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = CreateEntity(Transform(cameraPosition, Vector3(), 1.0f));
			sphere->AddComponent<HeightDespawn>();
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 0.5f));
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			rigidbody->AddForce<Force>(-(cameraRotation.ToQuaternion() * Vector3::FRONT).Normalize() * 3.0f, Time::Seconds(2.0f));
			sphere->AddComponent<ColliderSphere>();
		//	sphere->AddComponent<ColliderSphere>(0.5f, Transform(Vector3(0.0f, 1.0f, 0.0f)));
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<Light>(Colour::AQUA, 4.0f, Transform(Vector3(0.0f, 0.7f, 0.0f)));
			sphere->AddComponent<NameTag>("Sphere", 0.6f);
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();

		//	auto gizmoType1 = GizmoType::Resource(Model::Resource("Gizmos/Arrow.obj"), 3.0f);
		//	Gizmos::Get()->AddGizmo(new Gizmo(gizmoType1, Transform(cameraPosition, cameraRotation), Colour::PURPLE));

		//	auto collisionObject = sphere->GetComponent<CollisionObject>();
		//	collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){
		//		Log::Out("Sphere_Undefined collided with '%s'\n", other->GetParent()->GetName().c_str());});
		//	collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){
		//		Log::Out("Sphere_Undefined seperated with '%s'\n", other->GetParent()->GetName().c_str());});
		}

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

		if (m_buttonSave.WasDown())
		{
			auto sceneFile = FileJson("Scene1.json");
			auto sceneNode = sceneFile.GetParent()->AddChild(new Metadata("Scene"));

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

					auto componentName = Scenes::Get()->FindComponentName(component.get());

					if (componentName)
					{
						auto child = componentsNode->AddChild(new Metadata(*componentName));
						component->Encode(*child);
					}
				}
			}

			sceneFile.Save();
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
