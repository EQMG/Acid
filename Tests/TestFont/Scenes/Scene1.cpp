#include "Scene1.hpp"

#include <Animations/MeshAnimated.hpp>
#include <Renderer/Renderer.hpp>
#include <Uis/Uis.hpp>

namespace test
{
	Scene1::Scene1() :
		Scene(new Camera()),
		m_buttonFullscreen(ButtonKeyboard(Key::F11)),
		m_buttonScreenshot(ButtonKeyboard(Key::F12)),
		m_buttonExit(ButtonKeyboard(Key::Delete)),
		m_pannable(std::make_unique<Pannable>(Uis::Get()->GetContainer()))
	{
	}

	void Scene1::Start()
	{
	}

	void Scene1::Update()
	{
		if (m_buttonFullscreen.WasDown())
		{
			Window::Get()->SetFullscreen(!Window::Get()->IsFullscreen());
		}

		if (m_buttonScreenshot.WasDown())
		{
			std::string filename = "Screenshots/" + Engine::GetDateTime() + ".png";
			Renderer::Get()->CaptureScreenshot(filename);
		}

		if (m_buttonExit.WasDown())
		{
			Engine::Get()->RequestClose(false);
		}
	}

	bool Scene1::IsPaused() const
	{
		return true;
	}
}
