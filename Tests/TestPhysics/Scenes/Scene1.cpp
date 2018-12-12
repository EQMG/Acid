#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/Mouse.hpp>
#include <Lights/Light.hpp>
#include <Materials/MaterialDefault.hpp>
#include <Maths/Visual/DriverConstant.hpp>
#include <Maths/Visual/DriverSlide.hpp>
#include <Meshes/Mesh.hpp>
#include <Meshes/MeshRender.hpp>
#include <Models/Shapes/ModelSphere.hpp>
#include <Physics/ColliderCube.hpp>
#include <Physics/ColliderSphere.hpp>
#include <Models/Shapes/ModelCube.hpp>
#include <Physics/ColliderConvexHull.hpp>
#include <Scenes/Scenes.hpp>
#include <Models/Obj/ModelObj.hpp>
#include <Particles/ParticleSystem.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Shadows/ShadowRender.hpp>
#include <Renderer/Renderer.hpp>
#include <Physics/ColliderHeightfield.hpp>
#include <Terrain/Terrain.hpp>
#include <Terrain/MaterialTerrain.hpp>
#include <Physics/ColliderCapsule.hpp>
#include <Uis/Uis.hpp>
#include <Objects/Prefabs/PrefabObject.hpp>
#include <Helpers/FileSystem.hpp>
#include <Models/Shapes/ModelCylinder.hpp>
#include <Physics/ColliderCone.hpp>
#include <Physics/ColliderCylinder.hpp>
#include "Skybox/CelestialBody.hpp"
#include "Behaviours/HeightDespawn.hpp"
#include "Behaviours/NameTag.hpp"
#include "Behaviours/Rotate.hpp"
#include "FpsCamera.hpp"

namespace test
{
	static const Time UI_SLIDE_TIME = Time::Seconds(0.2f);

	Scene1::Scene1() :
		IScene(new FpsCamera(), new SelectorJoystick(JOYSTICK_1, 0, 1, {0, 1})),
		m_buttonSpawnSphere(ButtonMouse({MOUSE_BUTTON_1})),
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
		auto playerObject = new GameObject("Objects/Player/Player.xml", Transform(Vector3(0.0f, 2.0f, 0.0f), Vector3(0.0f, 180.0f, 0.0f)));

		// Skybox.
		auto skyboxObject = new GameObject("Objects/SkyboxClouds/SkyboxClouds.json", Transform(Vector3(), Vector3(), 2048.0f));

		// Animated.
	//	auto animatedObject = new GameObject(Transform(Vector3(0.0f, 2.0f, 0.0f), Vector3(), 0.25f));
	//	animatedObject->SetName("Animated");
	////	animatedObject->AddComponent<ColliderCapsule>(0.23f, 1.3f);
	////	animatedObject->AddComponent<Rigidbody>(0.1f, 0.7f);
	//	animatedObject->AddComponent<MeshAnimated>("Objects/Animated/Model.dae");
	//	animatedObject->AddComponent<MaterialDefault>(Colour::WHITE, Texture::Resource("Objects/Animated/Diffuse.png"), 0.7f, 0.6f);
	//	animatedObject->AddComponent<MeshRender>();
	//	animatedObject->AddComponent<ShadowRender>();

		// Entities.
		auto sun = new GameObject(Transform(Vector3(1000.0f, 5000.0f, -4000.0f), Vector3(), 18.0f));
		sun->SetName("Sun");
		sun->AddComponent<CelestialBody>(CELESTIAL_SUN);
		sun->AddComponent<Light>(Colour::WHITE);

		auto plane = new GameObject(Transform(Vector3(0.0f, -0.5f, 0.0f), Vector3(), Vector3(50.0f, 1.0f, 50.0f)));
		plane->SetName("Plane");
		plane->AddComponent<Mesh>(ModelCube::Resource(1.0f, 1.0f, 1.0f));
		plane->AddComponent<ColliderCube>(Vector3(1.0f, 1.0f, 1.0f));
		plane->AddComponent<Rigidbody>(0.0f, 0.5f);
		plane->AddComponent<MaterialDefault>(Colour::GREY, Texture::Resource("Undefined2.png"), 0.0f, 1.0f);
		plane->AddComponent<MeshRender>();
		plane->AddComponent<ShadowRender>();

		PrefabObject prefabPlane = PrefabObject("Plane.json");
		prefabPlane.Write(*plane);
		prefabPlane.Save();

		/*auto terrain = new GameObject(Transform());
		terrain->SetName("Terrain");
		terrain->AddComponent<Terrain>(150.0f, 2.0f);
		terrain->AddComponent<ColliderHeightfield>();
		terrain->AddComponent<Rigidbody>(0.0f, 0.7f);
		terrain->AddComponent<Mesh>();
		terrain->AddComponent<MaterialTerrain>(Texture::Resource("Objects/Terrain/Grass.png"), Texture::Resource("Objects/Terrain/Rocks.png"));
		terrain->AddComponent<MeshRender>();
		terrain->AddComponent<ShadowRender>();*/

		static const std::vector sphereColours = {Colour::RED, Colour::LIME, Colour::YELLOW, Colour::BLUE, Colour::PURPLE, Colour::GREY, Colour::WHITE};

		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				auto sphere = new GameObject(Transform(Vector3(i, j + 0.5f, -10.0f), Vector3(), 1.0f));
				sphere->SetName("Sphere_" + String::To(i) + "_" + String::To(j));
				sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 0.5f));
				sphere->AddComponent<ColliderSphere>();
				sphere->AddComponent<Rigidbody>(0.5f, 0.3f);
				sphere->AddComponent<MaterialDefault>(sphereColours[Maths::Random(0, sphereColours.size())]);
				sphere->AddComponent<MeshRender>();
				sphere->AddComponent<ShadowRender>();
			}
		}

		auto teapot = new GameObject(Transform(Vector3(6.0f, 1.0f, 10.0f), Vector3(), 0.2f));
		teapot->SetName("Teapot");
		teapot->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
	//	teapot->AddComponent<ColliderConvexHull>();
	//	teapot->AddComponent<Rigidbody>(1.0f);
		teapot->AddComponent<MaterialDefault>(Colour::FUCHSIA, nullptr, 0.0f, 1.0f);
		teapot->AddComponent<MeshRender>();
		teapot->AddComponent<ShadowRender>();
		teapot->AddComponent<Rotate>(Vector3(40.0f, 0.0f, 0.0f), false);

		auto teapot2 = new GameObject(Transform(Vector3(9.0f, 1.0f, 10.0f), Vector3(), 0.2f));
		teapot2->SetName("Teapot2");
		teapot2->AddComponent<Mesh>(ModelObj::Resource("Objects/Testing/Model_Tea.obj"));
	//	teapot2->AddComponent<ColliderConvexHull>();
	//	teapot2->AddComponent<Rigidbody>(1.0f);
		teapot2->AddComponent<MaterialDefault>(Colour::LIME, nullptr, 0.0f, 1.0f);
		teapot2->AddComponent<MeshRender>();
		teapot2->AddComponent<ShadowRender>();
		teapot2->AddComponent<Rotate>(Vector3(40.0f, 0.0f, 0.0f), true);

		auto cone = new GameObject(Transform(Vector3(-3.0f, 2.0f, 10.0f), Vector3(), 1.0f));
		cone->SetName("Cone");
		cone->AddComponent<Mesh>(ModelCylinder::Resource(1.0f, 0.0f, 2.0f, 16, 8));
		cone->AddComponent<ColliderCone>(1.0f, 2.0f);
		cone->AddComponent<Rigidbody>(1.5f);
		cone->AddComponent<MaterialDefault>(Colour::BLUE, nullptr, 0.0f, 1.0f);
		cone->AddComponent<MeshRender>();
		cone->AddComponent<ShadowRender>();

		auto cylinder = new GameObject(Transform(Vector3(-8.0f, 3.0f, 10.0f), Vector3(), 1.0f));
		cylinder->SetName("Cylinder");
		cylinder->AddComponent<Mesh>(ModelCylinder::Resource(1.1f, 1.1f, 2.2f, 16, 8));
		cylinder->AddComponent<ColliderCylinder>(1.1f, 2.2f);
		cylinder->AddComponent<Rigidbody>(2.5f);
		cylinder->AddComponent<MaterialDefault>(Colour::RED, nullptr, 0.0f, 1.0f);
		cylinder->AddComponent<MeshRender>();
		cylinder->AddComponent<ShadowRender>();

		auto smokeSystem = new GameObject("Objects/Smoke/Smoke.json", Transform(Vector3(-15.0f, 4.0f, 12.0f)));
	//	smokeSystem->AddComponent<Sound>("Sounds/Music/Hiitori-Bocchi.ogg", SOUND_TYPE_MUSIC, true, true);

		PrefabObject prefabSmokeSystem = PrefabObject("SmokeSystem.xml");
		prefabSmokeSystem.Write(*smokeSystem);
		prefabSmokeSystem.Save();
	}

	void Scene1::Update()
	{
		if (m_buttonSpawnSphere.WasDown())
		{
			Vector3 cameraPosition = Scenes::Get()->GetCamera()->GetPosition();
			Vector3 cameraRotation = Scenes::Get()->GetCamera()->GetRotation();

			auto sphere = new GameObject(Transform(cameraPosition, Vector3(), 1.0f));
			sphere->SetName("Sphere_Undefined");
			sphere->AddComponent<HeightDespawn>();
			sphere->AddComponent<Mesh>(ModelSphere::Resource(30, 30, 0.5f));
			sphere->AddComponent<ColliderSphere>();
			auto rigidbody = sphere->AddComponent<Rigidbody>(0.5f);
			rigidbody->AddForce<Force>((cameraRotation.ToQuaternion() * Vector3::FRONT).Normalize() * 3.0f, Time::Seconds(2.0f));
			sphere->AddComponent<MaterialDefault>(Colour::WHITE, nullptr, 0.0f, 1.0f);
			sphere->AddComponent<MeshRender>();
			sphere->AddComponent<ShadowRender>();
			sphere->AddComponent<Light>(Colour::AQUA, 4.0f, Vector3(0.0f, 0.7f, 0.0f));
			sphere->AddComponent<NameTag>(0.6f);

		//	auto collisionObject = sphere->GetComponent<CollisionObject>();
		//	collisionObject->GetCollisionEvents().Subscribe([&](CollisionObject *other){
		//		Log::Out("Sphere_Undefined collided with '%s'\n", other->GetGameObject()->GetName().c_str());});
		//	collisionObject->GetSeparationEvents().Subscribe([&](CollisionObject *other){
		//		Log::Out("Sphere_Undefined seperated with '%s'\n", other->GetGameObject()->GetName().c_str());});
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
