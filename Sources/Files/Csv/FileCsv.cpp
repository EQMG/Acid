#include "FileCsv.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FormatString.hpp"
#include "Helpers/FileSystem.hpp"

namespace fl
{
	FileCsv::FileCsv(const std::string &filename, const char &delimiter) :
		IFile(),
		m_filename(filename),
		m_delimiter(delimiter),
		m_rows(new std::vector<RowCsv>())
	{
	}

	FileCsv::~FileCsv()
	{
		delete m_rows;
	}

	void FileCsv::Load()
	{
#if FL_VERBOSE
		const auto debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::FileExists(m_filename))
		{
			fprintf(stderr, "File does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		std::string fileLoaded = FileSystem::ReadTextFile(m_filename);
		std::vector<std::string> lines = FormatString::Split(fileLoaded, "\n", true);

		for (auto &line : lines)
		{
			RowCsv row = RowCsv(FormatString::Split(line, std::string(1, m_delimiter), true));
			m_rows->push_back(row);
		}

#if FL_VERBOSE
		const auto debugEnd = Engine::Get()->GetTimeMs();
		printf("Json '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileCsv::Save()
	{
		std::string data = "";

		for (auto &row : *m_rows)
		{
			for (auto &element : row.m_elements)
			{
				data += element + m_delimiter;
			}

			data.pop_back();
			data += "\n";
		}

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, data);
	}

	void FileCsv::Clear()
	{
		m_rows->clear();
	}

	std::map<std::string, std::string> FileCsv::ConfigReadValues()
	{
		auto result = std::map<std::string, std::string>();

		for (unsigned int i = 0; i < m_rows->size(); i++)
		{
			RowCsv row = m_rows->at(i);
			result.insert(std::make_pair(row.m_elements.at(0), row.m_elements.at(1)));
		}

		return result;
	}

	void FileCsv::ConfigPushValue(const std::string &key, const std::string &value)
	{
		m_rows->push_back(RowCsv({key, value}));
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
			for (size_t i = m_rows->size(); i <= index; i++)
			{
				m_rows->push_back(RowCsv({}));
			}
		}

		m_rows->at(index).m_elements = row.m_elements;
	}

	void FileCsv::Verify()
	{
		if (!FileSystem::FileExists(m_filename))
		{
			FileSystem::CreateFile(m_filename);
		}
	}
}