#include "Plugins.hpp"

#include <thread>

#define CR_ROLLBACK 0
#define CR_HOST CR_DISABLE
#include <Engine/cr.h>

#include <Devices/Keyboard.hpp>
#include <Files/FileSystem.hpp>

namespace test
{
	Plugins::Plugins() :
		m_loadedPath(FileSystem::GetWorkingDirectory() + FileSystem::Separator + CR_PLUGIN("TestGUI")),
		m_watcher(FileWatcher(FileSystem::GetWorkingDirectory(), Time::Seconds(0.5f))),
		m_plugin(std::make_unique<cr_plugin>()),
		m_update(true)
	{
		cr_plugin_load(*m_plugin, m_loadedPath.c_str());

		// Watches the DLL path.
		m_watcher.GetOnChange() += [this](std::string path, FileWatcher::Status status)
		{
			if (path == m_loadedPath)
			{
				Log::Out("Reloading by change\n");
				m_update = true;
				//cr_plugin_update(*m_plugin);
			}
		};

		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, bitmask<InputMod> mods)
		{
			if (key == Key::L && action == InputAction::Press)
			{
				Log::Out("Reloading by key\n");
				//cr_plugin_unload(*m_plugin, false, false);
			}
		};
	}

	Plugins::~Plugins()
	{
		cr_plugin_close(*m_plugin);
	}

	void Plugins::Update()
	{
		if (m_update)
		{
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
			cr_plugin_update(*m_plugin);
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
			m_update = false;
		}
	}
}