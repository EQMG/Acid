#pragma once

#include <Scenes/Scene.hpp>
#include <Inputs/ButtonCompound.hpp>
#include "Uis/OverlayDebug.hpp"

using namespace acid;

namespace test
{
	class Scene1 :
		public Scene
	{
	public:
		Scene1();

		void Start() override;

		void Update() override;

		bool IsPaused() const override;

	private:
		ButtonCompound m_buttonPause;
		bool m_paused;

		std::unique_ptr<OverlayDebug> m_overlayDebug;
	};
}
