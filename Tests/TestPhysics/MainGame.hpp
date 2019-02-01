#pragma once

#include <Engine/Game.hpp>
#include <Files/FileWatcher.hpp>
#include "Configs/ConfigManager.hpp"

using namespace acid;

namespace test
{
	class MainGame :
		public Game
	{
	public:
		MainGame();

		~MainGame();

		void Start() override;

		void Update() override;
	private:
		std::unique_ptr<ConfigManager> m_configs;
		FileWatcher m_fileWatcher;
	};
}