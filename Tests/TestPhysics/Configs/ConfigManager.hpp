#pragma once

#include <Files/File.hpp>
#include <Helpers/Delegate.hpp>

using namespace acid;

namespace test {
class ConfigManager :
	public Observer {
public:
	ConfigManager();

	void Load();

	void Save() const;

private:
	File m_audio;
	File m_graphics;
};
}
