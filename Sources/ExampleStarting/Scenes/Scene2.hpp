#pragma once

#include <Sounds/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Scenes/Scene.hpp>

using namespace Flounder;

namespace Demo
{
	class Scene2 :
		public Scene
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonCaptureMouse;
		IButton *m_buttonScreenshot;
		IButton *m_buttonExit;

		Sound *m_soundScreenshot;
	public:
		Scene2();

		~Scene2();

		void Start() override;

		void Update() override;
	};
}
