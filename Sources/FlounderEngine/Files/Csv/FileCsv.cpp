#include "FileCsv.hpp"

#include "../../Helpers/HelperString.hpp"
#include "../../Helpers/HelperFile.hpp"

namespace Flounder
{
	FileCsv::FileCsv(const std::string &filename, const char &delimiter) :
		m_filename(filename),
		m_delimiter(delimiter),
		m_rows(new std::vector<RowCsv>())
	{
		Load();
	}

	FileCsv::~FileCsv()
	{
		Save();
		delete m_rows;
	}

	void FileCsv::Load()
	{
		Verify();
		std::string fileLoaded = HelperFile::ReadTextFile(m_filename);
		std::vector<std::string> lines = HelperString::Split(fileLoaded, "\n", true);

		for (std::string line : lines)
		{
			RowCsv row = RowCsv(HelperString::Split(line, std::string(1, m_delimiter), true));
			m_rows->push_back(row);
		}
	}

	void FileCsv::Save()
	{
		std::string data = "";

		for (RowCsv row : *m_rows)
		{
			for (std::string element : row.m_elements)
			{
				data += element + m_delimiter;
			}

			data.pop_back();
			data += "\n";
		}

		Verify();
		HelperFile::ClearFile(m_filename);
		HelperFile::WriteTextFile(m_filename, data);
	}

	void FileCsv::Verify()
	{
		if (!HelperFile::FileExists(m_filename))
		{
			HelperFile::CreateFile(m_filename);
		}
	}

	RowCsv FileCsv::GetRow(const unsigned int &index)
	{
		return m_rows->at(index);
	}

	void FileCsv::PushRow(const RowCsv &row)
	{
		m_rows->push_back(row);
	}

	void FileCsv::SetRow(const RowCsv &row, const unsigned int &index)
	{
		if (m_rows->size() <= index)
		{
			for (unsigned int i = m_rows->size(); i <= index; i++)
			{
				m_rows->push_back(RowCsv({}));
			}
		}

		m_rows->at(index).m_elements = row.m_elements;
	}

	void FileCsv::Clear()
	{
		m_rows->clear();
	}
}