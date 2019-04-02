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
	m_audio("Configs/Audio.yaml", new Yaml()),
	m_graphics("Configs/Graphics.yaml", new Yaml())
{
	Load();

	Events::Get()->AddEvent<EventTime>([&]() -> void
	{
		Save();
	}, Time::Seconds(2.5f));
}

void ConfigManager::Load()
{
	m_audio.Read();
	auto audioData = m_audio.GetMetadata();
	Audio::Get()->SetGain(Audio::Type::Master, audioData->GetChildDefault<float>("Master Volume", 1.0f));
	Audio::Get()->SetGain(Audio::Type::General, audioData->GetChildDefault<float>("General Volume", 1.0f));
	Audio::Get()->SetGain(Audio::Type::Effect, audioData->GetChildDefault<float>("Effect Volume", 1.0f));
	Audio::Get()->SetGain(Audio::Type::Music, audioData->GetChildDefault<float>("Music Volume", 1.0f));

	m_graphics.Read();
	auto graphicsData = m_graphics.GetMetadata();
	//Renderer::Get()->SetAntialiasing(graphicsData->GetChildDefault<bool>("Antialiasing", true));
	Window::Get()->SetSize(graphicsData->GetChildDefault<Vector2f>("Size", -Vector2i::One));
	Window::Get()->SetPosition(graphicsData->GetChildDefault<Vector2f>("Position", -Vector2i::One));
	Window::Get()->SetBorderless(graphicsData->GetChildDefault<bool>("Borderless", false));
	Window::Get()->SetResizable(graphicsData->GetChildDefault<bool>("Resizable", true));
	Window::Get()->SetFloating(graphicsData->GetChildDefault<bool>("Floating", false));
	Window::Get()->SetFullscreen(graphicsData->GetChildDefault<bool>("Fullscreen", false));
	Engine::Get()->SetFpsLimit(graphicsData->GetChildDefault<float>("FPS Limit", 0.0f));
}

void ConfigManager::Save()
{
	auto audioData = m_audio.GetMetadata();
	audioData->SetChild<float>("Master Volume", Audio::Get()->GetGain(Audio::Type::Master));
	audioData->SetChild<float>("General Volume", Audio::Get()->GetGain(Audio::Type::General));
	audioData->SetChild<float>("Effect Volume", Audio::Get()->GetGain(Audio::Type::Effect));
	audioData->SetChild<float>("Music Volume", Audio::Get()->GetGain(Audio::Type::Music));
	m_audio.Write();

	auto graphicsData = m_graphics.GetMetadata();
	//graphicsData->SetChild<bool>("Antialiasing", Renderer::Get()->IsAntialiasing());
	graphicsData->SetChild<Vector2f>("Size", Window::Get()->GetSize(false));
	graphicsData->SetChild<Vector2f>("Position", Window::Get()->GetPosition());
	graphicsData->SetChild<bool>("Borderless", Window::Get()->IsBorderless());
	graphicsData->SetChild<bool>("Resizable", Window::Get()->IsResizable());
	graphicsData->SetChild<bool>("Floating", Window::Get()->IsFloating());
	graphicsData->SetChild<bool>("Fullscreen", Window::Get()->IsFullscreen());
	graphicsData->SetChild<float>("FPS Limit", Engine::Get()->GetFpsLimit());
	m_graphics.Write();
}
}
