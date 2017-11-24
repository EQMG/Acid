#include "Instance.hpp"

#include "entities/EntityTreePine.hpp"
#include "entities/EntitySun.hpp"

namespace Demo
{
	Instance::Instance()
	{
		m_buttonFullscreen = new ButtonKeyboard({ GLFW_KEY_F11 });
		m_buttonLockMouse = new ButtonKeyboard({ GLFW_KEY_M });
		m_buttonExit = new ButtonKeyboard({ GLFW_KEY_DELETE });

#ifdef FLOUNDER_CONFIG_RELEASE
		Sound *music1 = new Sound("music", "res/music/outpost.wav");

		if (music1 != nullptr)
		{
			music1->SetPosition(Vector3(0.0f, 0.0f, 3.0f));
			music1->SetGain(0.9f);
			//	music1->Play();
			music1->Loop();
		}
#endif

		/*std::vector<particletype*> *types = new std::vector<particletype*>();
		types->push_back(new particletype("blue", texture::newTexture()->setFile("res/particles/blueParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
		types->push_back(new particletype("green", texture::newTexture()->setFile("res/particles/greenParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
		types->push_back(new particletype("purple", texture::newTexture()->setFile("res/particles/purpleParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
		types->push_back(new particletype("red", texture::newTexture()->setFile("res/particles/redParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
		types->push_back(new particletype("yellow", texture::newTexture()->setFile("res/particles/yellowParticle.png")->setNumberOfRows(4)->create(), 10.6f, 0.3f));
		m_systemTest = new particlesystem(types, new spawncircle(30.0f, Vector3(0.0f, 1.0f, 0.0f)), 1000, 1.0f, -0.1f);
		m_systemTest->setSystemCentre(Vector3(0.0f, 0.0f, 0.0f));
		particles::get()->addSystem(m_systemTest);*/

		Entity *sun = new EntitySun(nullptr, Transform(Vector3(), Vector3(), Vector3(16.0f, 16.0f, 16.0f))); // Entities::Get()->GetStructure()
		Entities::Get()->GetEntities()->push_back(sun);

		for (int i = -3; i <= 3; i++)
		{
			for (int j = -3; j <= 3; j++)
			{
				const float xv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				const float yv = 0.0f; //  Maths::RandomInRange(-10.0f, 10.0f);
				const Vector3 position = Terrains::Get()->GetPosition((20.0f * i) + xv, (20.0f * j) + yv);

				if (position.m_y > 0.0f)
				{
					Entity *test = new EntityTreePine(nullptr, Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), Vector3(1.0f, 1.0f, 1.0f))); // Entities::Get()->GetStructure()
					Entities::Get()->GetEntities()->push_back(test);
					
					if (i == 0 && j == 0)
					{
					//	test->AddComponent(new ComponentLight(Light(Colour("#2980b9"), Vector3(), Attenuation(0.0f, 0.01f, 1.0f))));
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
