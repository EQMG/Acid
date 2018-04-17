#pragma once

#include <Engine/IUpdater.hpp>
#include <Maths/Delta.hpp>
#include <Maths/Timer.hpp>

using namespace Flounder;

namespace Demo
{
	class MainUpdater :
		public IUpdater
	{
	private:
		Delta *m_deltaUpdate;
		Delta *m_deltaRender;
		Timer *m_timerUpdate;
		Timer *m_timerRender;

	public:
		MainUpdater();

		~MainUpdater();

		void Update(ModuleRegister *moduleRegister) override;

		float GetDelta() override { return m_deltaUpdate->GetChange(); }

		float GetDeltaRender() override { return m_deltaRender->GetChange(); }
	};
}