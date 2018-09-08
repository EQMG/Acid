#pragma once

#include <string>
#include "Files/IFile.hpp"
#include "RowCsv.hpp"

namespace acid
{
	class ACID_EXPORT FileCsv :
		public IFile
	{
	private:
		std::string m_filename;
		char m_delimiter;
		std::vector<RowCsv> m_rows;
	public:
		FileCsv(const std::string &filename, const char &delimiter = ',');

		~FileCsv();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::string GetFilename() const override { return m_filename; }

		void SetFilename(const std::string &filename) override { m_filename = filename; }

		Metadata *GetParent() const override { return nullptr; }

		char GetDelimiter() const { return m_delimiter; }

		void SetDelimiter(const char &delimiter) { m_delimiter = delimiter; }

		size_t GetRowCount() const { return m_rows.size(); }

		RowCsv &GetRow(const uint32_t &index);

		void PushRow(const RowCsv &row);

		void SetRow(const RowCsv &row, const uint32_t &index);

	private:
		void Verify();
	};
}
