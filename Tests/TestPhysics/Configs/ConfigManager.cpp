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

	ConfigManager::~ConfigManager()
	{
		Save();
	}

	void ConfigManager::Load()
	{
		m_audio->Load();
		auto audioNode = m_audio->GetParent();
		Audio::Get()->SetVolume(audioNode->GetChild<float>("Master Volume", 1.0f));

		m_graphics->Load();
		auto graphicsNode = m_graphics->GetParent();
		Display::Get()->SetWidth(graphicsNode->GetChild<uint32_t>("Display Width", 1080));
		Display::Get()->SetHeight(graphicsNode->GetChild<uint32_t>("Display Height", 720));
		Engine::Get()->SetFpsLimit(graphicsNode->GetChild<float>("Fps Limit", 0.0f));
		Display::Get()->SetAntialiasing(graphicsNode->GetChild<bool>("Is Antialiasing", true));
		Display::Get()->SetAntialiasing(graphicsNode->GetChild<bool>("Is Fullscreen", false));
	}

	void ConfigManager::Save()
	{
		auto audioNode = m_audio->GetParent();
		audioNode->SetChild<float>("Master Volume", Audio::Get()->GetVolume());
		m_audio->Save();

		auto graphicsNode = m_graphics->GetParent();
		graphicsNode->SetChild<uint32_t>("Display Width", Display::Get()->GetWidth());
		graphicsNode->SetChild<uint32_t>("Display Height", Display::Get()->GetHeight());
		graphicsNode->SetChild<float>("Fps Limit", Engine::Get()->GetFpsLimit());
		graphicsNode->SetChild<bool>("Is Antialiasing", Display::Get()->IsAntialiasing());
		graphicsNode->SetChild<bool>("Is Fullscreen", Display::Get()->IsAntialiasing());
		m_graphics->Save();
	}
}