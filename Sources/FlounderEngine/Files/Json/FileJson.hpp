#pragma once

#include <string>
#include "../IFile.hpp"

namespace Flounder
{
	class FileJson :
		public IFile
	{
	private:
		std::string m_filename;
	public:
		FileJson(const std::string &filename);

		~FileJson();

		void Load() override;

		void Save() override;

		void Clear() override;
		
		std::map<std::string, std::string> ConfigReadValues() override;
		
		void ConfigPushValue(const std::string &key, const std::string &value) override;
	private:
		void Verify();
	};
}