#include "Instance.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Entities/Entities.hpp>
#include <Terrains/Terrains.hpp>
#include <Maths/Maths.hpp>
#include <Devices/Display.hpp>
#include <Devices/Mouse.hpp>
#include <Sounds/Sound.hpp>
#include "Entities/EntityTreePine.hpp"
#include "Entities/EntityTesting.hpp"
#include "Entities/EntitySun.hpp"

namespace Demo
{
	Instance::Instance()
	{
		m_buttonFullscreen = new ButtonKeyboard({ GLFW_KEY_F11 });
		m_buttonLockMouse = new ButtonKeyboard({ GLFW_KEY_M });
		m_buttonExit = new ButtonKeyboard({ GLFW_KEY_DELETE });

#ifdef FLOUNDER_CONFIG_RELEASE
		Sound *music1 = new Sound("Resources/Music/Outpost.wav");

		if (music1 != nullptr)
		{
			music1->SetPosition(Vector3(0.0f, 0.0f, 3.0f));
			music1->SetGain(0.9f);
			//	music1->Play();
			music1->Loop();
		}
#endif

		/*std::vector<ParticleType*> *types = new std::vector<ParticleType*>();
		types->push_back(new ParticleType("blue", new Texture("Resources/Particles/ParticleBlue.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("purple", new Texture("Resources/Particles/ParticlePurple.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("red", new Texture("Resources/Particles/ParticleRed.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("yellow", new Texture("Resources/Particles/ParticleYellow.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		m_systemTest = new ParticleSystem(types, new SpawnCircle(30.0f, Vector3(0.0f, 1.0f, 0.0f)), 1000, 1.0f, -0.1f);
		m_systemTest->SetSystemCentre(Vector3(0.0f, 0.0f, 0.0f));
		Particles::Get()->AddSystem(m_systemTest);*/

		Entity *sun = new EntitySun(nullptr, Transform(Vector3(), Vector3(), Vector3(16.0f, 16.0f, 16.0f))); // Entities::Get()->GetStructure()
		Entities::Get()->GetEntities()->push_back(sun);

		for (int i = -5; i <= 0; i++)
		{
			for (int j = -5; j <= 0; j++)
			{
				const float xv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				const float yv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				Vector3 position = Terrains::Get()->GetPosition((20.0f * i) + xv, (20.0f * j) + yv);

				if (position.m_y > 0.0f)
				{
					Entity *test = nullptr;

					if (i < 0)
					{
						test = new EntityTreePine(nullptr, Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), Vector3(1.0f, 1.0f, 1.0f))); // Entities::Get()->GetStructure()
					}
					else
					{
						position.m_y += 1.0f;
						test = new EntityTesting(nullptr, Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), Vector3(1.0f, 1.0f, 1.0f))); // Entities::Get()->GetStructure()
					}

					Entities::Get()->GetEntities()->push_back(test);
				}
			}
		}
	}

	Instance::~Instance()
	{
		delete m_buttonFullscreen;
		delete m_buttonLockMouse;
		delete m_buttonExit;

		delete m_systemTest;
	}

	void Instance::Update()
	{
		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonLockMouse->WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}
}
