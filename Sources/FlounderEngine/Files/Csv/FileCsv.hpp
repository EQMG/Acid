#pragma once

#include <string>
#include <utility>
#include <vector>

namespace Flounder
{
	class RowCsv
	{
	public:
		std::vector<std::string> m_elements;

		RowCsv(std::vector<std::string> elements) :
			m_elements(std::move(elements))
		{
		}
	};

	class FileCsv
	{
	private:
		std::string m_filename;
		char m_delimiter;
		std::vector<RowCsv> *m_rows;
	public:
		FileCsv(const std::string &filename, const char &delimiter = ',');

		~FileCsv();

		void Load();

		void Save();

		unsigned int GetRowCount() const { return m_rows->size(); }

		RowCsv GetRow(const unsigned int &index);

		void PushRow(const RowCsv &row);

		void SetRow(const RowCsv &row, const unsigned int &index);

		void Clear();
	private:
		void Verify();
	};
}