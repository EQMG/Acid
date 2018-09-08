#pragma once

#include <string>
#include <vector>
#include "Engine/Exports.hpp"

namespace acid
{
	class ACID_EXPORT RowCsv
	{
	private:
		std::vector<std::string> m_elements;
	public:
		RowCsv(const std::vector<std::string> &elements);

		std::vector<std::string> GetElements() const { return m_elements; }

		void AddElement(const std::string &element) { m_elements.emplace_back(element); }

		void ClearElements() { m_elements.clear(); }

		void SetElements(const std::vector<std::string> &elements) { m_elements = elements; }
	};
}
