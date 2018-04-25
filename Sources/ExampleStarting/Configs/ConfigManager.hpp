#pragma once

#include <Files/Config.hpp>

using namespace fl;

namespace Demo
{
	class ConfigManager
	{
	private:
		Config *m_configAudio;
		Config *m_configGraphics;
	public:
		ConfigManager();

		~ConfigManager();

		void Save();
	};
}