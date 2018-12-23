#pragma once

#include <Scenes/Scene.hpp>
#include <Inputs/ButtonKeyboard.hpp>
#include "Uis/Pannable.hpp"

using namespace acid;

namespace test
{
	class Scene1 :
		public Scene
	{
	private:
		ButtonKeyboard m_buttonFullscreen;
		ButtonKeyboard m_buttonScreenshot;
		ButtonKeyboard m_buttonExit;

		std::unique_ptr<Pannable> m_pannable;
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;
	};
}
