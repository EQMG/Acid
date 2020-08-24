#pragma once

#include <Engine/Engine.hpp>
#include <Files/FileObserver.hpp>
#include <Files/Files.hpp>
#include <Inputs/Buttons/KeyboardInputButton.hpp>
#include "Uis/Panels.hpp"

using namespace acid;

struct cr_plugin;

namespace test {
/**
 * Module used for managing the world.
 */
class Plugins : public Module::Registrar<Plugins>, public rocket::trackable {
	static const bool Registered;
public:
	Plugins();
	~Plugins();

	void Update() override;

private:
	std::filesystem::path loadedPath;
	FileObserver fileObserver;
	std::unique_ptr<cr_plugin> plugin;
	bool update = true;

	//Panels panels;

	KeyboardInputButton buttonReload;
};
}
