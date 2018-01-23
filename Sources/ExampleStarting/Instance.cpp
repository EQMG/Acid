#include "Instance.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Entities/Entities.hpp>
#include <Terrains/Terrains.hpp>
#include <Maths/Maths.hpp>
#include <Devices/Display.hpp>
#include <Devices/Mouse.hpp>

/*#ifdef FLOUNDER_CONFIG_RELEASE
#include <Sounds/Sound.hpp>
#endif*/

#include "Entities/EntityTreePine.hpp"
#include "Entities/EntityTesting.hpp"
#include "Entities/EntitySun.hpp"

namespace Demo
{
	Instance::Instance() :
		m_buttonFullscreen(new ButtonKeyboard({GLFW_KEY_F11})),
		m_buttonLockMouse(new ButtonKeyboard({GLFW_KEY_M})),
		m_buttonExit(new ButtonKeyboard({GLFW_KEY_DELETE}))
	{
/*#ifdef FLOUNDER_CONFIG_RELEASE
		Sound *music1 = Sound::Resource("Resources/Music/Outpost.wav");

		if (music1 != nullptr)
		{
			music1->SetPosition(Vector3(3.0f, 0.0f, 3.0f));
			music1->SetGain(0.9f);
			//	music1->Play();
			music1->Loop();
		}
#endif*/

		/*std::vector<ParticleType*> *types = new std::vector<ParticleType*>();
		types->push_back(new ParticleType("blue", Texture::Resource("Resources/Particles/Blue/Texture.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("purple", Texture::Resource("Resources/Particles/Purple/Texture.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("red", Texture::Resource("Resources/Particles/Red/Texture.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		types->push_back(new ParticleType("yellow", Texture::Resource("Resources/Particles/Yellow/Texture.png", false, false, 1, true, false, 4), 10.6f, 0.3f));
		m_systemTest = new ParticleSystem(types, new SpawnCircle(30.0f, Vector3(0.0f, 10.0f, 0.0f)), 1000, 1.0f, -0.1f);
		m_systemTest->SetSystemCentre(Vector3(0.0f, 0.0f, 0.0f));
		Particles::Get()->AddSystem(m_systemTest);*/

		new EntitySun(Entities::Get()->GetStructure(), Transform(Vector3(), Vector3(), Vector3(10.0f, 10.0f, 10.0f)));

		for (int i = -5; i <= 0; i++)
		{
			for (int j = -5; j <= 0; j++)
			{
				const float xv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				const float yv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				Vector3 position = Terrains::Get()->GetPosition((20.0f * i) + xv, (20.0f * j) + yv);

				if (position.m_y > 0.0f)
				{
					if (i < 0)
					{
						new EntityTreePine(Entities::Get()->GetStructure(), Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), Vector3(1.0f, 1.0f, 1.0f)));
					}
					else
					{
						position.m_y += 1.0f;
						new EntityTesting(Entities::Get()->GetStructure(), Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), Vector3(1.0f, 1.0f, 1.0f)));
					}
				}
			}
		}
	}

	Instance::~Instance()
	{
		delete m_buttonFullscreen;
		delete m_buttonLockMouse;
		delete m_buttonExit;

		//	delete m_systemTest;
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
