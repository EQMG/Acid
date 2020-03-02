#pragma once

#include <Files/File.hpp>
#include <Helpers/Delegate.hpp>

using namespace acid;

namespace test {
class ConfigManager : public virtual Observer {
public:
	ConfigManager();

	void Load();
	void Save() const;

private:
	File audio;
	File graphics;
};
}
