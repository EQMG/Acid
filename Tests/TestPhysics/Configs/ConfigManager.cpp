#include "ConfigManager.hpp"

#include <Devices/Window.hpp>
#include <Graphics/Graphics.hpp>
#include <Audio/Audio.hpp>
#include <Timers/Timers.hpp>
#include <Serialized/Json/Json.hpp>

namespace test {
ConfigManager::ConfigManager() :
	m_audio("Configs/Audio.json", std::make_unique<Json>()),
	m_graphics("Configs/Graphics.json", std::make_unique<Json>()) {
	Load();
	Timers::Get()->Every(160s, [this]() {
		Save();
	}, this);
}

void ConfigManager::Load() {
	/*m_audio.Load();
	auto &audioData = m_audio.GetNode();
	Audio::Get()->SetGain(Audio::Type::Master, audioData["masterVolume"].Get<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::General, audioData["generalVolume"].Get<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::Effect, audioData["effectVolume"].Get<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::Music, audioData["musicVolume"].Get<float>(1.0f));

	m_graphics.Load();
	auto &graphicsData = m_graphics.GetNode();
	//Renderer::Get()->SetAntialiasing(graphicsData["antialiasing"].Get<bool>(true));
	Window::Get()->SetSize(graphicsData["size"].Get<Vector2f>(Vector2i(-1)));
	Window::Get()->SetPosition(graphicsData["position"].Get<Vector2f>(Vector2i(-1)));
	Window::Get()->SetBorderless(graphicsData["borderless"].Get<bool>(false));
	Window::Get()->SetResizable(graphicsData["resizable"].Get<bool>(true));
	Window::Get()->SetFloating(graphicsData["floating"].Get<bool>(false));
	Window::Get()->SetFullscreen(graphicsData["fullscreen"].Get<bool>(false));
	Engine::Get()->SetFpsLimit(graphicsData["fpsLimit"].Get<float>(0.0f));*/
}

void ConfigManager::Save() const {
	/*auto &audioData = m_audio.GetNode();
	audioData["masterVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Master));
	audioData["generalVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::General));
	audioData["effectVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Effect));
	audioData["musicVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Music));
	m_audio.Write(Node::Format::Beautified);

	auto &graphicsData = m_graphics.GetNode();
	//graphicsData["antialiasing"].Set<bool>(Renderer::Get()->IsAntialiasing());
	graphicsData["size"].Set<Vector2f>(Window::Get()->GetSize(false));
	graphicsData["position"].Set<Vector2f>(Window::Get()->GetPosition());
	graphicsData["borderless"].Set<bool>(Window::Get()->IsBorderless());
	graphicsData["resizable"].Set<bool>(Window::Get()->IsResizable());
	graphicsData["floating"].Set<bool>(Window::Get()->IsFloating());
	graphicsData["fullscreen"].Set<bool>(Window::Get()->IsFullscreen());
	graphicsData["fpsLimit"].Set<float>(Engine::Get()->GetFpsLimit());
	m_graphics.Write(Node::Format::Beautified);*/
}
}
