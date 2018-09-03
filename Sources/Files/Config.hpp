#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include "Helpers/FormatString.hpp"
#include "IFile.hpp"

namespace acid
{
	class ACID_EXPORT Config
	{
	private:
		std::shared_ptr<IFile> m_file;
	public:
		Config(const std::shared_ptr<IFile> &file);

		~Config();

		virtual void Decode(Serialized &serialized) = 0;

		virtual void Encode(Serialized &serialized) const = 0;

		void Load();

		void Save();
	};
}
