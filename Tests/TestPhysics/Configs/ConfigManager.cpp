#include "ConfigManager.hpp"

#include <Helpers/FileSystem.hpp>
#include <Serialized/Json/Json.hpp>
#include <Renderer/Renderer.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Audio/Audio.hpp>

namespace test
{
	ConfigManager::ConfigManager() :
		m_audio(std::make_unique<File>("Configs/Audio.json", new Json())),
		m_graphics(std::make_unique<File>("Configs/Graphics.json", new Json()))
	{
		Load();

		Events::Get()->AddEvent<EventTime>([&]() -> void
		{
			Save();
		}, Time::Seconds(2.5f));
	}

	void ConfigManager::Load()
	{
		m_audio->Read();
		auto audioData = m_audio->GetMetadata();
		Audio::Get()->SetMasterGain(audioData->GetChild<float>("Master Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_GENERAL, audioData->GetChild<float>("General Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_EFFECT, audioData->GetChild<float>("Effect Volume", 1.0f));
		Audio::Get()->SetTypeGain(SOUND_TYPE_MUSIC, audioData->GetChild<float>("Music Volume", 1.0f));

		m_graphics->Read();
		auto graphicsData = m_graphics->GetMetadata();
	//	Renderer::Get()->SetAntialiasing(graphicsData->GetChild<bool>("Antialiasing", true));
		Window::Get()->SetDimensions(graphicsData->GetChild<Vector2>("Dimensions", -Vector2::ONE));
		Window::Get()->SetPosition(graphicsData->GetChild<Vector2>("Position", -Vector2::ONE));
		Window::Get()->SetBorderless(graphicsData->GetChild<bool>("Borderless", false));
		Window::Get()->SetResizable(graphicsData->GetChild<bool>("Resizable", true));
		Window::Get()->SetFloating(graphicsData->GetChild<bool>("Floating", false));
		Window::Get()->SetFullscreen(graphicsData->GetChild<bool>("Fullscreen", false));
		Engine::Get()->SetFpsLimit(graphicsData->GetChild<float>("FPS Limit", 0.0f));
	}

	void ConfigManager::Save()
	{
		auto audioData = m_audio->GetMetadata();
		audioData->SetChild<float>("Master Volume", Audio::Get()->GetMasterGain());
		audioData->SetChild<float>("General Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_GENERAL));
		audioData->SetChild<float>("Effect Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_EFFECT));
		audioData->SetChild<float>("Music Volume", Audio::Get()->GetTypeGain(SOUND_TYPE_MUSIC));
		m_audio->Write();

		auto graphicsData = m_graphics->GetMetadata();
	//	graphicsData->SetChild<bool>("Antialiasing", Renderer::Get()->IsAntialiasing());
		graphicsData->SetChild<Vector2>("Dimensions", Window::Get()->GetDimensions());
		graphicsData->SetChild<Vector2>("Position", Window::Get()->GetPosition());
		graphicsData->SetChild<bool>("Borderless", Window::Get()->IsBorderless());
		graphicsData->SetChild<bool>("Resizable", Window::Get()->IsResizable());
		graphicsData->SetChild<bool>("Floating", Window::Get()->IsFloating());
		graphicsData->SetChild<bool>("Fullscreen", Window::Get()->IsFullscreen());
		graphicsData->SetChild<float>("FPS Limit", Engine::Get()->GetFpsLimit());
		m_graphics->Write();
	}
}