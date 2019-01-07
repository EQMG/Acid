#pragma once

#include <string>
#include <vector>
#include "Files/IFile.hpp"
#include "JsonSection.hpp"

namespace acid
{
	class ACID_EXPORT FileJson :
		public IFile
	{
	private:
		std::string m_filename;
		std::unique_ptr<Metadata> m_parent;
	public:
		explicit FileJson(const std::string &filename);

		void Load() override;

		void Save() override;

		void Clear() override;

		const std::string &GetFilename() const override { return m_filename; }

		void SetFilename(const std::string &filename) override { m_filename = filename; }

		Metadata *GetParent() const override { return m_parent.get(); }

		Metadata *GetChild(const std::string &name) const { return m_parent->FindChild(name); }
	private:
		void Verify();
	};
}
