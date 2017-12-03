#pragma once

#include <Flounder.hpp>

using namespace Flounder;

namespace Demo
{
	class Instance :
		public IStandard
	{
	private:
		IButton *m_buttonFullscreen;
		IButton *m_buttonLockMouse;
		IButton *m_buttonExit;

		ParticleSystem *m_systemTest;
	public:
		Instance();

		~Instance();

		void Update() override;
	};
}
