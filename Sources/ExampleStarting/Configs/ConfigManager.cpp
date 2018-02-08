#include "ConfigManager.hpp"

#include <Helpers/FileSystem.hpp>
#include <Devices/Display.hpp>
#include <Devices/Audio.hpp>

namespace Demo
{
	ConfigManager::ConfigManager()
	{
		FileSystem::CreateFolder(FileSystem::GetWorkingDirectory() + "/Configs");

		m_configAudio = new Config(new FileCsv(FileSystem::GetWorkingDirectory() + "/Configs/Audio.csv"));
		m_configAudio->Load();
		m_configAudio->Link<float>("MasterVolume", 1.0f, nullptr, nullptr);

		m_configGraphics = new Config(new FileCsv(FileSystem::GetWorkingDirectory() + "/Configs/Graphics.csv"));
		m_configGraphics->Load();
		//	m_configGraphics->Link<float>("FpsLimit", 0.0f, Display::Get()->GetFpsLimit, Display::Get()->SetFpsLimit);
		//	m_configGraphics->Link<bool>("Antialiasing", true, Display::Get()->IsAntialiasing, Display::Get()->SetAntialiasing);
		//	m_configGraphics->Link<bool>("Fullscreen", false, Display::Get()->IsFullscreen, Display::Get()->SetFullscreen);
		//	m_configGraphics->Link<int>("DisplayWidth", false, Display::Get()->GetWidth, nullptr);
		//	m_configGraphics->Link<int>("DisplayHeight", false, Display::Get()->GetHeight, nullptr);
	}

	ConfigManager::~ConfigManager()
	{
		m_configAudio->Save();
		delete m_configAudio;

		m_configGraphics->Save();
		delete m_configGraphics;
	}
}