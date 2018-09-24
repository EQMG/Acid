#include "FileCsv.hpp"

#include "Engine/Engine.hpp"
#include "Helpers/FileSystem.hpp"

namespace acid
{
	FileCsv::FileCsv(const std::string &filename, const char &delimiter) :
		m_filename(filename),
		m_delimiter(delimiter),
		m_rows(std::vector<RowCsv>())
	{
	}

	void FileCsv::Load()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		if (!FileSystem::Exists(m_filename))
		{
			Log::Error("File does not exist: '%s'\n", m_filename.c_str());
			return;
		}

		auto fileLoaded = FileSystem::ReadTextFile(m_filename);

		if (!fileLoaded)
		{
			return;
		}

		auto lines = String::Split(*fileLoaded, "\n", true);

		for (auto &line : lines)
		{
			RowCsv row = RowCsv(String::Split(line, std::string(1, m_delimiter), true));
			m_rows.emplace_back(row);
		}

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Csv '%s' loaded in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileCsv::Save()
	{
#if ACID_VERBOSE
		float debugStart = Engine::Get()->GetTimeMs();
#endif

		std::stringstream builder;

		for (auto &row : m_rows)
		{
			for (auto &element : row.GetElements())
			{
				builder << element;

				if (element != row.GetElements().back())
				{
					builder << m_delimiter;
				}
			}

			builder << "\n";
		}

		Verify();
		FileSystem::ClearFile(m_filename);
		FileSystem::WriteTextFile(m_filename, builder.str());

#if ACID_VERBOSE
		float debugEnd = Engine::Get()->GetTimeMs();
		Log::Out("Csv '%s' saved in %fms\n", m_filename.c_str(), debugEnd - debugStart);
#endif
	}

	void FileCsv::Clear()
	{
		m_rows.clear();
	}

	RowCsv &FileCsv::GetRow(const uint32_t &index)
	{
		return m_rows.at(index);
	}

	void FileCsv::PushRow(const RowCsv &row)
	{
		m_rows.emplace_back(row);
	}

	void FileCsv::SetRow(const RowCsv &row, const uint32_t &index)
	{
		if (m_rows.size() <= index)
		{
			for (size_t i = m_rows.size(); i <= index; i++)
			{
				m_rows.emplace_back(RowCsv({}));
			}
		}

		m_rows.at(index).SetElements(row.GetElements());
	}

	void FileCsv::Verify()
	{
		if (!FileSystem::Exists(m_filename))
		{
			FileSystem::Create(m_filename);
		}
	}
}
