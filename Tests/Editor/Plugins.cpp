#include "Plugins.hpp"

#define CR_HOST CR_SAFE

#include <Engine/cr.h>
#include <Devices/Keyboard.hpp>
#include <Graphics/Graphics.hpp>
#include <Scenes/Scenes.hpp>
#include <Uis/Uis.hpp>

namespace test
{
Plugins::Plugins() :
	m_loadedPath{std::filesystem::current_path() / CR_PLUGIN("EditorTest")},
	m_watcher{std::filesystem::current_path(), 0.5s},
	m_plugin{std::make_unique<cr_plugin>()},
	m_update{true},
	m_panels{&Uis::Get()->GetCanvas()},
	m_buttonReload{Key::R}
{
	auto pathStr = m_loadedPath.string();
	std::replace(pathStr.begin(), pathStr.end(), '\\', '/');
	cr_plugin_load(*m_plugin, pathStr.c_str());

	// Watches the plugin path.
	m_watcher.OnChange().Add([this](std::filesystem::path path, FileWatcher::Status status)
	{
		if (path == m_loadedPath)
		{
			m_update = true;
		}
	});

	m_buttonReload.OnButton().Add([this](InputAction action, BitMask<InputMod> mods)
	{
		if (action == InputAction::Press)
		{
			//std::filesystem::last_write_time(m_loadedPath, std::filesystem::file_time_type{Time::Now()});
		}
	});
}

Plugins::~Plugins()
{
	cr_plugin_close(*m_plugin);
}

void Plugins::Update()
{
	if (m_update)
	{
		Log::Out("[Host] Updating plugin\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(150));
		m_panels.SetParent(nullptr);
		cr_plugin_unload(*m_plugin, false, false);
		cr_plugin_update(*m_plugin);
		m_panels.SetParent(&Uis::Get()->GetCanvas());
		m_update = false;
	}
}
}
