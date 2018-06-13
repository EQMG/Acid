#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include <Inputs/Mouse.hpp>
#include <Renderer/Screenshot/Screenshot.hpp>
#include "ManagerUis.hpp"
#include "FpsCamera.hpp"

namespace test
{
	Scene1::Scene1() :
		IScene(new FpsCamera(), new ManagerUis()),
		m_buttonFullscreen(new ButtonKeyboard({Key::KEY_F11})),
		m_buttonScreenshot(new ButtonKeyboard({Key::KEY_F12})),
		m_buttonExit(new ButtonKeyboard({Key::KEY_DELETE}))
	{
	}

	Scene1::~Scene1()
	{
		delete m_buttonFullscreen;
		delete m_buttonScreenshot;
		delete m_buttonExit;
	}

	void Scene1::Start()
	{
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen->WasDown())
		{
			Display::Get()->SetFullscreen(!Display::Get()->IsFullscreen());
		}

		if (m_buttonScreenshot->WasDown())
		{
			std::string filename = "Screenshots/" + Engine::Get()->GetDateTime() + ".png";
			Screenshot::Capture(filename);
		}

		if (m_buttonExit->WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}
}
