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
		m_audio(std::make_unique<FileJson>("Configs/Audio.json")),
		m_graphics(std::make_unique<FileJson>("Configs/Graphics.json"))
	{
		Load();

		Events::Get()->AddEvent<EventTime>([&]() -> void
		{
			Save();
		}, Time::Seconds(2.5f));
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
		Display::Get()->SetDimensions(graphicsData->GetChild<Vector2>("Dimensions", -Vector2::ONE));
		Display::Get()->SetPosition(graphicsData->GetChild<Vector2>("Position", -Vector2::ONE));
		Display::Get()->SetAntialiasing(graphicsData->GetChild<bool>("Antialiasing", true));
		Display::Get()->SetBorderless(graphicsData->GetChild<bool>("Borderless", false));
		Display::Get()->SetResizable(graphicsData->GetChild<bool>("Resizable", true));
		Display::Get()->SetFloating(graphicsData->GetChild<bool>("Floating", false));
		Display::Get()->SetFullscreen(graphicsData->GetChild<bool>("Fullscreen", false));
		Engine::Get()->SetFpsLimit(graphicsData->GetChild<float>("FPS Limit", 0.0f));
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
		graphicsData->SetChild<Vector2>("Dimensions", Display::Get()->GetDimensions());
		graphicsData->SetChild<Vector2>("Position", Display::Get()->GetPosition());
		graphicsData->SetChild<bool>("Antialiasing", Display::Get()->IsAntialiasing());
		graphicsData->SetChild<bool>("Borderless", Display::Get()->IsBorderless());
		graphicsData->SetChild<bool>("Resizable", Display::Get()->IsResizable());
		graphicsData->SetChild<bool>("Floating", Display::Get()->IsFloating());
		graphicsData->SetChild<bool>("Fullscreen", Display::Get()->IsFullscreen());
		graphicsData->SetChild<float>("FPS Limit", Engine::Get()->GetFpsLimit());
		m_graphics->Save();
	}
}