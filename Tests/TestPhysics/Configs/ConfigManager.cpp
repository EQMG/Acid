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
		m_audio(std::make_unique<FileJson>(FileSystem::GetWorkingDirectory() + "/Configs/Audio.json")),
		m_graphics(std::make_unique<FileJson>(FileSystem::GetWorkingDirectory() + "/Configs/Graphics.json"))
	{
		FileSystem::CreateFolder("Configs");
		Load();

		Events::Get()->AddEvent<EventTime>(2.5f, [&]() -> void
		{
			Save();
		}, false);
	}

	void ConfigManager::Load()
	{
		m_audio->Load();
		auto audioData = m_audio->GetParent();
		Audio::Get()->SetVolume(audioData->GetChild<float>("Master Volume", 1.0f));

		m_graphics->Load();
		auto graphicsData = m_graphics->GetParent();
		Display::Get()->SetWidth(graphicsData->GetChild<uint32_t>("Display Width", 1080));
		Display::Get()->SetHeight(graphicsData->GetChild<uint32_t>("Display Height", 720));
		Engine::Get()->SetFpsLimit(graphicsData->GetChild<float>("Fps Limit", 0.0f));
		Display::Get()->SetAntialiasing(graphicsData->GetChild<bool>("Is Antialiasing", true));
		Display::Get()->SetAntialiasing(graphicsData->GetChild<bool>("Is Fullscreen", false));
	}

	void ConfigManager::Save()
	{
		auto audioData = m_audio->GetParent();
		audioData->SetChild<float>("Master Volume", Audio::Get()->GetVolume());
		m_audio->Save();

		auto graphicsData = m_graphics->GetParent();
		graphicsData->SetChild<uint32_t>("Display Width", Display::Get()->GetWidth());
		graphicsData->SetChild<uint32_t>("Display Height", Display::Get()->GetHeight());
		graphicsData->SetChild<float>("Fps Limit", Engine::Get()->GetFpsLimit());
		graphicsData->SetChild<bool>("Is Antialiasing", Display::Get()->IsAntialiasing());
		graphicsData->SetChild<bool>("Is Fullscreen", Display::Get()->IsAntialiasing());
		m_graphics->Save();
	}
}