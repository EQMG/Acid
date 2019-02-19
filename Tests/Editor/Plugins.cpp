#include "Plugins.hpp"

#define CR_HOST CR_SAFE
#include <Engine/cr.h>
#include <Devices/Keyboard.hpp>
#include <Files/FileSystem.hpp>
#include <Renderer/Renderer.hpp>
#include <Scenes/Scenes.hpp>

namespace test
{
	Plugins::Plugins() :
		m_loadedPath(FileSystem::GetWorkingDirectory() + FileSystem::Separator + CR_PLUGIN("EditorTest")),
		m_watcher(FileWatcher(FileSystem::GetWorkingDirectory(), Time::Seconds(0.5f))),
		m_plugin(std::make_unique<cr_plugin>()),
		m_update(true),
		m_panels(std::make_unique<Panels>(Uis::Get()->GetContainer()))
	{
		cr_plugin_load(*m_plugin, m_loadedPath.c_str());

		// Watches the DLL path.
		m_watcher.GetOnChange() += [this](std::string path, FileWatcher::Status status)
		{
			if (path == m_loadedPath)
			{
				m_update = true;
			}
		};

		Keyboard::Get()->GetOnKey() += [this](Key key, InputAction action, bitmask<InputMod> mods)
		{
			if (key == Key::R && action == InputAction::Press)
			{
				FileSystem::Touch(m_loadedPath);
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
			Log::Out("[Host] Updating plugin\n");
			std::this_thread::sleep_for(std::chrono::milliseconds(150));
			m_panels->SetParent(nullptr);
			cr_plugin_unload(*m_plugin, false, false);
			cr_plugin_update(*m_plugin);
			m_panels->SetParent(Uis::Get()->GetContainer());
			m_update = false;
		}
	}
}
