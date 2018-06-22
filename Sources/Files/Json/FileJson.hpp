#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Helpers/FormatString.hpp"
#include "Files/IFile.hpp"
#include "JsonSection.hpp"

namespace fl
{
	class FL_EXPORT FileJson :
		public IFile
	{
	private:
		std::string m_filename;
		std::shared_ptr<LoadedValue> m_parent;
	public:
		FileJson(const std::string &filename);

		~FileJson();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::map<std::string, std::string> ConfigReadValues() override;

		void ConfigPushValue(const std::string &key, const std::string &value) override;

		std::shared_ptr<LoadedValue> GetParent() const { return m_parent; }

		std::shared_ptr<LoadedValue> GetChild(const std::string &name) const { return m_parent->GetChild(name); }
	private:
		void Verify();
	};
}