#include "Instance.hpp"

#include <Inputs/ButtonKeyboard.hpp>
#include <Entities/Entities.hpp>
#include <Terrains/Terrains.hpp>
#include <Maths/Maths.hpp>
#include <Devices/Display.hpp>
#include <Devices/Mouse.hpp>
#include <Helpers/FileSystem.hpp>
#include <Particles/Spawns/SpawnCircle.hpp>
#include <Particles/Particles.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>

namespace Demo
{
	Instance::Instance() :
		m_buttonFullscreen(new ButtonKeyboard({GLFW_KEY_F11})),
		m_buttonCaptureMouse(new ButtonKeyboard({GLFW_KEY_M})),
		m_buttonScreenshot(new ButtonKeyboard({GLFW_KEY_F12})),
		m_buttonExit(new ButtonKeyboard({GLFW_KEY_DELETE})),
		m_soundScreenshot(new Sound("Resources/Sounds/Screenshot.ogg"))
	{
		new Entity("Sun", Transform(Vector3(), Vector3(), 18.0f));
		new Entity("Moon", Transform(Vector3(), Vector3(), 9.0f));

		Vector3 foundationCentre = Terrains::Get()->GetPosition(15.7f, -25.0f);

		for (int i = -3; i <= 0; i++)
		{
			for (int j = -3; j <= 0; j++)
			{
				new Entity("FoundationWood", Transform(foundationCentre, Vector3(), 1.5f)); //  + Vector3(i * 3.0f, 2.3f, -j * 3.0f)
			}
		}

		for (int i = -8; i <= 0; i++)
		{
			for (int j = -8; j <= 0; j++)
			{
				const float xv = Maths::RandomInRange(-3.0f, 3.0f);
				const float yv = Maths::RandomInRange(-3.0f, 3.0f);
				Vector3 position = Terrains::Get()->GetPosition((13.0f * i) + xv, (13.0f * j) + yv);

				if (position.m_y > 0.0f)
				{
					if (i < 0)
					{
						position.m_y -= 1.0f;
						new Entity("TreePine", Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), 1.0f));
					}
					else
					{
						position.m_y += 1.0f;
						new Entity("Testing", Transform(position, Vector3(0.0f, Maths::RandomInRange(0.0f, 360.0f), 0.0f), 1.0f));
					}
				}
			}
		}

/*#ifdef FLOUNDER_CONFIG_RELEASE
		Sound *music1 = new Sound("Resources/Music/Outpost.ogg", 0.9f);

		music1->SetPosition(Vector3(0.0f, 7.0f, 0.0f));
		music1->Loop();
#endif*/

/*#ifdef FLOUNDER_CONFIG_DEBUG
		std::vector<ParticleType*> *types = new std::vector<ParticleType*>();
		types->push_back(new ParticleType("blue", Texture::Resource("Resources/Particles/Blue/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("purple", Texture::Resource("Resources/Particles/Purple/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("red", Texture::Resource("Resources/Particles/Red/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		types->push_back(new ParticleType("yellow", Texture::Resource("Resources/Particles/Yellow/Texture.png"), 10.6f, 0.3f)); // , false, false, 1, true, false, 4
		ParticleSystem *systemTest = new ParticleSystem(types, new SpawnCircle(30.0f, Vector3(0.0f, 10.0f, 0.0f)), 1000, 1.0f, -0.1f);
		systemTest->SetSystemCentre(Vector3(0.0f, 0.0f, 0.0f));
		Particles::Get()->AddSystem(systemTest);
#endif*/
	}

	Instance::~Instance()
	{
		delete m_buttonFullscreen;
		delete m_buttonCaptureMouse;
		delete m_buttonScreenshot;
		delete m_buttonExit;
	}

	void Instance::Update()
	{
		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonCaptureMouse->WasDown())
		{
			Mouse::Get()->SetCursorHidden(!Mouse::Get()->IsCursorDisabled());
		}

		if (m_buttonScreenshot->WasDown())
		{
			std::string filename = "Screenshots/" + Engine::Get()->GetDateTime() + ".png";

			m_soundScreenshot->Play();
			Screenshot::Capture(filename);
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}
}
