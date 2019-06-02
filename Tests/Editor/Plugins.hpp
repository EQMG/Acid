#pragma once

#include <Engine/Engine.hpp>
#include <Files/FileWatcher.hpp>
#include "Uis/Panels.hpp"
#include "Inputs/ButtonKeyboard.hpp"

using namespace acid;

struct cr_plugin;

namespace test
{
/**
 * Module used for managing the world.
 */
class Plugins :
	public Module
{
public:
	/**
	 * Gets the engines instance.
	 * @return The current module instance.
	 */
	static Plugins *Get() { return Engine::Get()->GetModule<Plugins>(); }

	Plugins();

	~Plugins();

	void Update() override;

private:
	std::filesystem::path m_loadedPath;
	FileWatcher m_watcher;
	std::unique_ptr<cr_plugin> m_plugin;
	bool m_update;

	Panels m_panels;

	ButtonKeyboard m_buttonReload;
};
}
