#include "ConfigManager.hpp"

#include <Display/Display.hpp>
#include <Helpers/FileSystem.hpp>
#include <Files/Json/FileJson.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Audio/Audio.hpp>
#include <Events/EventChange.hpp>

namespace test
{
	ConfigManager::ConfigManager() :
		m_audio(std::make_unique<FileJson>("Configs/Audio.json")),
		m_graphics(std::make_unique<FileJson>("Configs/Graphics.json"))
	{
		Load();

		Events::Get()->AddEvent<EventTime>(Time::Seconds(2.5f), [&]() -> void
		{
			Save();
		}, false);

		//	Events::Get()->AddEvent(new EventChange<uint32_t>([](){
		//		return Display::Get()->GetWidth();
		//	}, [](uint32_t value){
		//		Log::Out("New width: %i\n", value);
		//	}));
	}

	void ConfigManager::Load()
	{
		m_audio->Load();
		auto audioData = m_audio->GetParent();
		Audio::Get()->SetMasterGain(audioData->GetChild<float>("Master Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_GENERAL, audioData->GetChild<float>("General Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_EFFECT, audioData->GetChild<float>("Effect Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_MUSIC, audioData->GetChild<float>("Music Volume", 1.0f));

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
		audioData->SetChild<float>("Master Volume", Audio::Get()->GetMasterGain());
		audioData->SetChild<float>("General Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_GENERAL));
		audioData->SetChild<float>("Effect Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_EFFECT));
		audioData->SetChild<float>("Music Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_MUSIC));
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