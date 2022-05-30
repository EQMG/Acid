#include "Plugins.hpp"

#define CR_HOST CR_SAFE
#include <cr.h>

#include <Devices/Keyboard.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>

namespace test {
Plugins::Plugins() :
	loadedPath(std::filesystem::current_path() / CR_PLUGIN("EditorTest")),
	fileObserver(loadedPath, 0.5s),
	plugin(std::make_unique<cr_plugin>()),
	buttonReload(Key::R) {
	//panels.SetTransform({UiMargins::All});
	//Uis::Get()->GetCanvas().AddChild(&panels);
	
	auto pathStr = loadedPath.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	cr_plugin_load(*plugin, pathStr.c_str());

	// Watches the plugin path.
	fileObserver.OnChange().connect(this, [this](std::filesystem::path path, FileObserver::Status status) {
		update = true;
	});
	buttonReload.OnButton().connect(this, [this](InputAction action, bitmask::bitmask<InputMod> mods) {
		if (action == InputAction::Press) {
			//std::filesystem::last_write_time(loadedPath, std::filesystem::file_time_type(Time::Now()));
			update = true;
		}
	});
}

Plugins::~Plugins() {
	cr_plugin_close(*plugin);
}

void Plugins::Update() {
	if (update) {
		Log::Debug("[Host] Updating plugin\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(150));
		//panels.SetParent(nullptr);
		cr_plugin_unload(*plugin, false, false);
		cr_plugin_update(*plugin);
		//panels.SetParent(&Uis::Get()->GetCanvas());
		update = false;
	}
}
}
