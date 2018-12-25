/*#pragma once

#include "Outline.hpp"

namespace acid
{
	class Cell
	{
	public:
		Rect bbox;
		uint32_t value;
		uint32_t from;
		uint32_t to;
		uint32_t startLength;

		uint32_t AddRange(const uint32_t &cell, const uint32_t &from, const uint32_t &to) const;

		bool AddBezier(const Outline &o, const Outline &u, const uint32_t &i, const uint32_t &j,
			const uint32_t &contourIndex);

		bool FinishContour(const Outline &o, const Outline &u, const uint32_t &contour_index, uint32_t &maxStartLength);

		static bool CellsAddBezier(const Outline &o, const Outline &u, const uint32_t &i, const uint32_t &j,
			const uint32_t &contourIndex, Cell *cells);

		static bool CellsFinishContour(const Outline &o, const Outline &u, const uint32_t &contour_index, Cell *cells,
			uint32_t &maxStartLength);

		static void CellsSetFilled(const Outline &u, Cell *cells, const uint32_t &filledCell);
	};
}*/
