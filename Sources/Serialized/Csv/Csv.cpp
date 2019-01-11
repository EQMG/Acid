#include "Csv.hpp"

namespace acid
{
	Csv::Csv(const char &delimiter) :
		Metadata("", ""),
		m_delimiter(delimiter),
		m_rows(std::vector<RowCsv>())
	{
	}

	void Csv::Load(const std::string &data)
	{
		auto lines = String::Split(data, "\n", true);

		for (const auto &line : lines)
		{
			RowCsv row = RowCsv(String::Split(line, std::string(1, m_delimiter), true));
			m_rows.emplace_back(row);
		}
	}

	std::string Csv::Write() const
	{
		std::stringstream builder;

		for (const auto &row : m_rows)
		{
			for (const auto &element : row.GetElements())
			{
				builder << element;

				if (element != row.GetElements().back())
				{
					builder << m_delimiter;
				}
			}

			builder << "\n";
		}

		return builder.str();
	}

	void Csv::Clear()
	{
		ClearChildren();
		m_rows.clear();
	}

	RowCsv &Csv::GetRow(const uint32_t &index)
	{
		return m_rows.at(index);
	}

	void Csv::PushRow(const RowCsv &row)
	{
		m_rows.emplace_back(row);
	}

	void Csv::SetRow(const RowCsv &row, const uint32_t &index)
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
}
