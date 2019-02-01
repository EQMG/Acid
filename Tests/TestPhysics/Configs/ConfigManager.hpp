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
		std::unique_ptr<File> m_audio;
		std::unique_ptr<File> m_graphics;
	};
}