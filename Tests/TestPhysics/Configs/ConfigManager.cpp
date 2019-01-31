#include "ConfigManager.hpp"

#include <Files/FileSystem.hpp>
#include <Serialized/Yaml/Yaml.hpp>
#include <Renderer/Renderer.hpp>
#include <Events/Events.hpp>
#include <Events/EventTime.hpp>
#include <Audio/Audio.hpp>

namespace test
{
	ConfigManager::ConfigManager() :
		m_audio(std::make_unique<File>("Configs/Audio.yaml", new Yaml())),
		m_graphics(std::make_unique<File>("Configs/Graphics.yaml", new Yaml()))
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
		Audio::Get()->SetMasterGain(audioData->GetChildDefault<float>("Master Volume", 1.0f));
		Audio::Get()->SetTypeGain(Audio::Type::General, audioData->GetChildDefault<float>("General Volume", 1.0f));
		Audio::Get()->SetTypeGain(Audio::Type::Effect, audioData->GetChildDefault<float>("Effect Volume", 1.0f));
		Audio::Get()->SetTypeGain(Audio::Type::Music, audioData->GetChildDefault<float>("Music Volume", 1.0f));

		m_graphics->Read();
		auto graphicsData = m_graphics->GetMetadata();
	//	Renderer::Get()->SetAntialiasing(graphicsData->GetChildDefault<bool>("Antialiasing", true));
		Window::Get()->SetDimensions(graphicsData->GetChildDefault<Vector2>("Dimensions", -Vector2::One));
		Window::Get()->SetPosition(graphicsData->GetChildDefault<Vector2>("Position", -Vector2::One));
		Window::Get()->SetBorderless(graphicsData->GetChildDefault<bool>("Borderless", false));
		Window::Get()->SetResizable(graphicsData->GetChildDefault<bool>("Resizable", true));
		Window::Get()->SetFloating(graphicsData->GetChildDefault<bool>("Floating", false));
		Window::Get()->SetFullscreen(graphicsData->GetChildDefault<bool>("Fullscreen", false));
		Engine::Get()->SetFpsLimit(graphicsData->GetChildDefault<float>("FPS Limit", 0.0f));
	}

	void ConfigManager::Save()
	{
		auto audioData = m_audio->GetMetadata();
		audioData->SetChild<float>("Master Volume", Audio::Get()->GetMasterGain());
		audioData->SetChild<float>("General Volume", Audio::Get()->GetTypeGain(Audio::Type::General));
		audioData->SetChild<float>("Effect Volume", Audio::Get()->GetTypeGain(Audio::Type::Effect));
		audioData->SetChild<float>("Music Volume", Audio::Get()->GetTypeGain(Audio::Type::Music));
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