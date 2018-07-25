#pragma once

#include <Files/Config.hpp>

using namespace acid;

namespace test
{
	class ConfigManager
	{
	private:
		Config m_configAudio;
		Config m_configGraphics;
	public:
		ConfigManager();

		~ConfigManager();

		void Save();
	};
}