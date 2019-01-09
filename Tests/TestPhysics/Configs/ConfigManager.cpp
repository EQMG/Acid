#include "ConfigManager.hpp"

#include <Helpers/FileSystem.hpp>
#include <Files/Json/FileJson.hpp>
#include <Renderer/Renderer.hpp>
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
		Renderer::Get()->SetAntialiasing(graphicsData->GetChild<bool>("Antialiasing", true));
		Renderer::Get()->GetWindow()->SetDimensions(graphicsData->GetChild<Vector2>("Dimensions", -Vector2::ONE));
		Renderer::Get()->GetWindow()->SetPosition(graphicsData->GetChild<Vector2>("Position", -Vector2::ONE));
		Renderer::Get()->GetWindow()->SetBorderless(graphicsData->GetChild<bool>("Borderless", false));
		Renderer::Get()->GetWindow()->SetResizable(graphicsData->GetChild<bool>("Resizable", true));
		Renderer::Get()->GetWindow()->SetFloating(graphicsData->GetChild<bool>("Floating", false));
		Renderer::Get()->GetWindow()->SetFullscreen(graphicsData->GetChild<bool>("Fullscreen", false));
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
		graphicsData->SetChild<bool>("Antialiasing", Renderer::Get()->IsAntialiasing());
		graphicsData->SetChild<Vector2>("Dimensions", Renderer::Get()->GetWindow()->GetDimensions());
		graphicsData->SetChild<Vector2>("Position", Renderer::Get()->GetWindow()->GetPosition());
		graphicsData->SetChild<bool>("Borderless", Renderer::Get()->GetWindow()->IsBorderless());
		graphicsData->SetChild<bool>("Resizable", Renderer::Get()->GetWindow()->IsResizable());
		graphicsData->SetChild<bool>("Floating", Renderer::Get()->GetWindow()->IsFloating());
		graphicsData->SetChild<bool>("Fullscreen", Renderer::Get()->GetWindow()->IsFullscreen());
		graphicsData->SetChild<float>("FPS Limit", Engine::Get()->GetFpsLimit());
		m_graphics->Save();
	}
}