#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"
#include "LoadedValue.hpp"

namespace acid
{
	class ACID_EXPORT IFile
	{
	public:
		IFile()
		{
		};

		virtual ~IFile()
		{
		};

		virtual void Load() = 0;

		virtual void Save() = 0;

		virtual void Clear() = 0;

		virtual std::map<std::string, std::string> ConfigReadValues() = 0;

		virtual void ConfigPushValue(const std::string &key, const std::string &value) = 0;

		virtual std::string GetFilename() const = 0;

		virtual void SetFilename(const std::string &filename) = 0;

		virtual LoadedValue *GetParent() const = 0;
	};
}
