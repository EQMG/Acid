#pragma once

#include <Files/DataFile.hpp>

using namespace acid;

namespace test
{
	class ConfigManager
	{
	private:
		std::unique_ptr<DataFile> m_audio;
		std::unique_ptr<DataFile> m_graphics;
	public:
		ConfigManager();

		void Load();

		void Save();
	};
}