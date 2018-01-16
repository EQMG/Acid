#pragma once

#include <string>
#include <map>

namespace Flounder
{
	class IFile
	{
	public:
		virtual ~IFile()
		{
		};

		virtual void Load() = 0;

		virtual void Save() = 0;
		
		virtual void Clear() = 0;
		
		virtual std::map<std::string, std::string> ConfigReadValues() = 0;
		
		virtual void ConfigPushValue(const std::string &key, const std::string &value) = 0;
	};
}
