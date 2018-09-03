#include "ConfigManager.hpp"

#include <Display/Display.hpp>
#include <Helpers/FileSystem.hpp>
#include <Files/Json/FileJson.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Audio/Audio.hpp>

namespace test
{
	ConfigManager::ConfigManager() :
		Config(std::make_shared<FileJson>(FileSystem::GetWorkingDirectory() + "/Config.json"))
	{
	//	FileSystem::CreateFolder("Configs");
		Load();

		Events::Get()->AddEvent<EventTime>(2.5f, false, [&]() -> void
		{
			Save();
		});
	}

	ConfigManager::~ConfigManager()
	{
		Save();
	}

	void ConfigManager::Decode(Serialized &serialized)
	{
	//	Audio::Get()->SetVolume(serialized.GetChild<float>("Master Volume", 1.0f));

		Display::Get()->SetWidth(serialized.GetChild<uint32_t>("Display Width", 1080));
		Display::Get()->SetHeight(serialized.GetChild<uint32_t>("Display Height", 720));
		Engine::Get()->SetFpsLimit(serialized.GetChild<float>("Fps Limit", 0.0f));
		Display::Get()->SetAntialiasing(serialized.GetChild<bool>("Is Antialiasing", true));
		Display::Get()->SetAntialiasing(serialized.GetChild<bool>("Is Fullscreen", false));
	}

	void ConfigManager::Encode(Serialized &serialized) const
	{
	//	serialized.SetChild<float>("Master Volume", Audio::Get()->GetVolume());

		serialized.SetChild<uint32_t>("Display Width", Display::Get()->GetWidth());
		serialized.SetChild<uint32_t>("Display Height", Display::Get()->GetHeight());
		serialized.SetChild<float>("Fps Limit", Engine::Get()->GetFpsLimit());
		serialized.SetChild<bool>("Is Antialiasing", Display::Get()->IsAntialiasing());
		serialized.SetChild<bool>("Is Fullscreen", Display::Get()->IsAntialiasing());
	}
}