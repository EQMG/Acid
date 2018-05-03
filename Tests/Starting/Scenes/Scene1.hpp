#pragma once

#include <Audio/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Scenes/Scene.hpp>

using namespace fl;

namespace Demo
{
	class Scene1 :
		public Scene
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonCaptureMouse;
		IButton *m_buttonScreenshot;
		IButton *m_buttonExit;

		Sound *m_soundScreenshot;
	public:
		Scene1();

		~Scene1();

		void Start() override;

		void Update() override;
	};
}
