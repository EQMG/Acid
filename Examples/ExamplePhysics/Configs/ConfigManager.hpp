#pragma once

#include <rocket.hpp>

#include <Files/File.hpp>

using namespace acid;

namespace test {
class ConfigManager : public virtual rocket::trackable {
public:
	ConfigManager();

	void Load();
	void Save();

private:
	File audio;
	File graphics;
};
}
