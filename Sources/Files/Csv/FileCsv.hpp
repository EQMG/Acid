#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Files/IFile.hpp"

namespace fl
{
	class FL_EXPORT RowCsv
	{
	private:
		std::vector<std::string> m_elements;
	public:
		RowCsv(const std::vector<std::string> &elements) :
			m_elements(std::vector<std::string>(elements))
		{
		}

		std::vector<std::string> GetElements() const { return m_elements; }

		void SetElements(const std::vector<std::string> &elements) { m_elements = elements; }
	};

	class FL_EXPORT FileCsv :
		public IFile
	{
	private:
		std::string m_filename;
		char m_delimiter;
		std::vector<RowCsv> *m_rows;
	public:
		FileCsv(const std::string &filename, const char &delimiter = ',');

		~FileCsv();

		void Load() override;

		void Save() override;

		void Clear() override;

		std::map<std::string, std::string> ConfigReadValues() override;

		void ConfigPushValue(const std::string &key, const std::string &value) override;

		size_t GetRowCount() const { return m_rows->size(); }

		RowCsv GetRow(const unsigned int &index);

		void PushRow(const RowCsv &row);

		void SetRow(const RowCsv &row, const unsigned int &index);

	private:
		void Verify();
	};
}