#include "ConfigManager.hpp"

#include <Helpers/FileSystem.hpp>
#include <Devices/Display.hpp>
#include <Devices/Audio.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>

namespace Demo
{
	ConfigManager::ConfigManager()
	{
		FileSystem::CreateFolder("Configs");

		m_configAudio = new Config(new FileCsv(FileSystem::GetWorkingDirectory() + "/Configs/Audio.csv"));
		m_configAudio->Load();
	//	m_configAudio->Link<float>("MasterVolume", 1.0f, nullptr, nullptr);

		m_configGraphics = new Config(new FileCsv(FileSystem::GetWorkingDirectory() + "/Configs/Graphics.csv"));
		m_configGraphics->Load();
		m_configGraphics->Link<float>("FpsLimit", 0.0f, CONFIG_GET(Display::Get()->GetFpsLimit()), CONFIG_SET(float, Display::Get()->SetFpsLimit(v)));
		m_configGraphics->Link<bool>("Antialiasing", true, CONFIG_GET(Display::Get()->IsAntialiasing()), CONFIG_SET(bool, Display::Get()->SetAntialiasing(v)));
		m_configGraphics->Link<bool>("Fullscreen", false, CONFIG_GET(Display::Get()->IsFullscreen()), CONFIG_SET(bool, Display::Get()->SetFullscreen(v)));
		m_configGraphics->Link<int>("DisplayWidth", 1080, CONFIG_GET(Display::Get()->GetWidth()));
		m_configGraphics->Link<int>("DisplayHeight", 720, CONFIG_GET(Display::Get()->GetHeight()));

		Events::Get()->AddEvent(new EventTime(2.5f, false, [&]() -> void { Save(); }));
	}

	ConfigManager::~ConfigManager()
	{
		Save();

		delete m_configAudio;
		delete m_configGraphics;
	}

	void ConfigManager::Save()
	{
		printf("Saving config manager\n");
		m_configAudio->Save();
		m_configGraphics->Save();
	}
}