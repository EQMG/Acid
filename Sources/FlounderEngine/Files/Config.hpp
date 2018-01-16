#pragma once

#include <string>
#include <map>
#include "IFile.hpp"

namespace Flounder
{
	class Config
	{
	private:
		IFile *m_file;
		std::map<std::string, std::string> *m_values;
	public:
		Config(IFile *file);

		~Config();

		void Load();

		void Save();

		std::string Value(const std::string &key, const std::string &normal = "nullptr");

		bool Value(const std::string &key, const bool &normal = false);

		int Value(const std::string &key, const int &normal = 0);

		float Value(const std::string &key, const float &normal = 0.0f);

		double Value(const std::string &key, const double &normal = 0.0);
	};
}