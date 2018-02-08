#pragma once

#include <Files/Config.hpp>
#include <Files/Csv/FileCsv.hpp>

using namespace Flounder;

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
	};
}