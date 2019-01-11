#pragma once

#include "Serialized/Metadata.hpp"
#include "RowCsv.hpp"

namespace acid
{
	class ACID_EXPORT Csv :
		public Metadata
	{
	private:
		char m_delimiter;
		std::vector<RowCsv> m_rows;
	public:
		explicit Csv(const char &delimiter = ',');

		void Load(const std::string &data) override;

		std::string Write() const override;

		void Clear() override;

		const char &GetDelimiter() const { return m_delimiter; }

		void SetDelimiter(const char &delimiter) { m_delimiter = delimiter; }

		RowCsv &GetRow(const uint32_t &index);

		size_t GetRowCount() const { return m_rows.size(); }

		void PushRow(const RowCsv &row);

		void SetRow(const RowCsv &row, const uint32_t &index);
	};
}
