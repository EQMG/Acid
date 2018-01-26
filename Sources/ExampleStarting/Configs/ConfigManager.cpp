#include "ConfigManager.hpp"

#include <Helpers/FileSystem.hpp>
#include <Devices/Display.hpp>

namespace Demo
{
	ConfigManager::ConfigManager()
	{
		FileSystem::CreateFolder(FileSystem::GetWorkingDirectory() + "/Configs");
		m_configGraphics = new Config(new FileCsv(FileSystem::GetWorkingDirectory() + "/Configs/Graphics.csv"));
		m_configGraphics->Load();
	}

	ConfigManager::~ConfigManager()
	{
		m_configGraphics->Set("FpsLimit", Display::Get()->GetFpsLimit());
		m_configGraphics->Set("Antialiasing", Display::Get()->IsAntialiasing());
		m_configGraphics->Set("Fullscreen", Display::Get()->IsFullscreen());
		m_configGraphics->Save();
		delete m_configGraphics;
	}
}