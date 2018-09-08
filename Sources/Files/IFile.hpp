#pragma once

#include <map>
#include <string>
#include "Engine/Exports.hpp"
#include "Serialized/Metadata.hpp"

namespace acid
{
	class ACID_EXPORT IFile
	{
	public:
		virtual void Load() = 0;

		virtual void Save() = 0;

		virtual void Clear() = 0;

		virtual std::string GetFilename() const = 0;

		virtual void SetFilename(const std::string &filename) = 0;

		virtual Metadata *GetParent() const = 0;
	};
}
