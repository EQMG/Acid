#pragma once

#include <Files/File.hpp>

using namespace acid;

namespace test
{
class ConfigManager
{
public:
	ConfigManager();

	void Load();

	void Save();

private:
	File m_audio;
	File m_graphics;
};
}
