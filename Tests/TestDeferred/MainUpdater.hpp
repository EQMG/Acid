#pragma once

#include <Engine/IUpdater.hpp>
#include <Maths/Delta.hpp>
#include <Maths/Timer.hpp>

using namespace acid;

namespace test
{
	class MainUpdater :
		public IUpdater
	{
	private:
		Delta m_deltaUpdate;
		Delta m_deltaRender;
		Timer m_timerUpdate;
		Timer m_timerRender;

	public:
		MainUpdater();

		~MainUpdater();

		void Update(const ModuleRegister &moduleRegister) override;

		float GetDelta() override { return m_deltaUpdate.GetChange(); }

		float GetDeltaRender() override { return m_deltaRender.GetChange(); }
	};
}