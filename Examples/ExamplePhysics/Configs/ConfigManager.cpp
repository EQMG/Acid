#include "ConfigManager.hpp"

#include <Devices/Windows.hpp>
#include <Graphics/Graphics.hpp>
#include <Files/Json/Json.hpp>
#include <Timers/Timers.hpp>

import acid.sound;

namespace test {
ConfigManager::ConfigManager() :
	audio("Configs/Audio.json", std::make_unique<Json>()),
	graphics("Configs/Graphics.json", std::make_unique<Json>()) {
	Timers::Get()->Every(this, [this]() {
		Save();
	}, 160s);
}

void ConfigManager::Load() {
	audio.Load();
	auto &audioData = audio.GetNode();
	Audio::Get()->SetGain(Audio::Type::Master, audioData["masterVolume"].GetWithFallback<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::General, audioData["generalVolume"].GetWithFallback<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::Effect, audioData["effectVolume"].GetWithFallback<float>(1.0f));
	Audio::Get()->SetGain(Audio::Type::Music, audioData["musicVolume"].GetWithFallback<float>(1.0f));

	graphics.Load();
	Windows::Get()->OnAddWindow().connect(this, [this](Window *window, bool added) {
		auto &graphicsData = graphics.GetNode();
		//Renderer::Get()->SetAntialiasing(graphicsData["antialiasing"].GetWithFallback<bool>(true));
		window->SetSize(graphicsData["size"].GetWithFallback<Vector2f>(Vector2i(1080, 720)));
		//window->SetPosition(graphicsData["position"].GetWithFallback<Vector2f>(Vector2i(0, 0)));
		window->SetBorderless(graphicsData["borderless"].GetWithFallback<bool>(false));
		window->SetResizable(graphicsData["resizable"].GetWithFallback<bool>(true));
		window->SetFloating(graphicsData["floating"].GetWithFallback<bool>(false));
		window->SetFullscreen(graphicsData["fullscreen"].GetWithFallback<bool>(false));
		Engine::Get()->SetFpsLimit(graphicsData["fpsLimit"].GetWithFallback<float>(-1.0f));
	});

#ifdef ACID_DEBUG
	Save();
#endif
}

void ConfigManager::Save() {
	auto &audioData = audio.GetNode();
	audioData["masterVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Master));
	audioData["generalVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::General));
	audioData["effectVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Effect));
	audioData["musicVolume"].Set<float>(Audio::Get()->GetGain(Audio::Type::Music));
	audio.Write(NodeFormat::Beautified);

	if (auto window = Windows::Get()->GetWindow(0)) {
		auto &graphicsData = graphics.GetNode();
		//graphicsData["antialiasing"].Set<bool>(Renderer::Get()->IsAntialiasing());
		graphicsData["size"].Set<Vector2f>(window->GetSize(false));
		//graphicsData["position"].Set<Vector2f>(window->GetPosition());
		graphicsData["borderless"].Set<bool>(window->IsBorderless());
		graphicsData["resizable"].Set<bool>(window->IsResizable());
		graphicsData["floating"].Set<bool>(window->IsFloating());
		graphicsData["fullscreen"].Set<bool>(window->IsFullscreen());
		graphicsData["fpsLimit"].Set<float>(Engine::Get()->GetFpsLimit());
	}
	graphics.Write(NodeFormat::Beautified);
}
}
