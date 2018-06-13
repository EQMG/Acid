#pragma once

#include <Inputs/IButton.hpp>
#include <Scenes/IScene.hpp>

using namespace fl;

namespace test
{
	class Scene1 :
		public IScene
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonScreenshot;
		IButton *m_buttonExit;
	public:
		Scene1();

		~Scene1();

		void Start() override;

		void Update() override;
	};
}
