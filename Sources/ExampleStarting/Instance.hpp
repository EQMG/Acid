#pragma once

#include <Sounds/Sound.hpp>
#include <Inputs/IButton.hpp>
#include <Standards/IStandard.hpp>

using namespace Flounder;

namespace Demo
{
	class Instance :
		public IStandard
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonCaptureMouse;
		IButton *m_buttonScreenshot;
		IButton *m_buttonExit;

		Sound *m_soundScreenshot;
	public:
		Instance();

		~Instance();

		void Update() override;
	};
}
