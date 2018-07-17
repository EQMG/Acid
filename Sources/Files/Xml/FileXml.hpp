#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Files/IFile.hpp"
#include "Files/LoadedValue.hpp"

namespace fl
{
	class FL_EXPORT FileXml :
		public IFile
	{
	private:
		std::string m_filename;
		LoadedValue *m_parent;
	public:
		FileXml(const std::string &filename);

		~FileXml();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::map<std::string, std::string> ConfigReadValues() override;

		void ConfigPushValue(const std::string &key, const std::string &value) override;

		LoadedValue *GetParent() const { return m_parent; }

		LoadedValue *GetChild(const std::string &name) const { return m_parent->GetChild(name); }
	private:
		void Verify();
	};
}