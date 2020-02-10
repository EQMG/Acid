#include "Plugins.hpp"

#define CR_HOST CR_SAFE
#include <cr/cr.h>

#include <Devices/Keyboard.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>

namespace test {
Plugins::Plugins() :
	m_loadedPath(std::filesystem::current_path() / CR_PLUGIN("EditorTest")),
	m_fileObserver(m_loadedPath, 0.5s),
	m_plugin(std::make_unique<cr_plugin>()),
	m_buttonReload(Key::R) {
	//m_panels.SetTransform({UiMargins::All});
	//Uis::Get()->GetCanvas().AddChild(&m_panels);
	
	auto pathStr = m_loadedPath.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	cr_plugin_load(*m_plugin, pathStr.c_str());

	// Watches the plugin path.
	m_fileObserver.OnChange().Add([this](std::filesystem::path path, FileObserver::Status status) {
		m_update = true;
	}, this);
	m_buttonReload.OnButton().Add([this](InputAction action, BitMask<InputMod> mods) {
		if (action == InputAction::Press) {
			//std::filesystem::last_write_time(m_loadedPath, std::filesystem::file_time_type(Time::Now()));
			m_update = true;
		}
	}, this);
}

Plugins::~Plugins() {
	cr_plugin_close(*m_plugin);
}

void Plugins::Update() {
	if (m_update) {
		Log::Debug("[Host] Updating plugin\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(150));
		//m_panels.SetParent(nullptr);
		cr_plugin_unload(*m_plugin, false, false);
		cr_plugin_update(*m_plugin);
		//m_panels.SetParent(&Uis::Get()->GetCanvas());
		m_update = false;
	}
}
}
