/*#include "Cell.hpp"

#include <assert.h>

namespace acid
{
	uint32_t Cell::AddRange(const uint32_t &cell, const uint32_t &from, const uint32_t &to) const
	{
		assert(from % 2 == 0 && to % 2 == 0);

		uint32_t rangeCell = cell;
		uint32_t rangeFrom = from;
		uint32_t rangeTo = to;

		rangeFrom /= 2;
		rangeTo /= 2;

		if (rangeFrom >= std::numeric_limits<uint8_t>::max())
		{
			return 0;
		}

		if (rangeTo >= std::numeric_limits<uint8_t>::max())
		{
			return 0;
		}

		uint32_t length = rangeTo - rangeFrom;

		if (length <= 3 && (cell & 0x03) == 0)
		{
			rangeCell |= rangeFrom << 8;
			rangeCell |= length;
			return rangeCell;
		}

		if (length > 7)
		{
			return 0;
		}

		if ((rangeCell & 0x1C) == 0)
		{
			rangeCell |= rangeFrom << 16;
			rangeCell |= length << 2;
			return rangeCell;
		}

		if ((rangeCell & 0xE0) == 0)
		{
			rangeCell |= rangeFrom << 24;
			rangeCell |= length << 5;
			return rangeCell;
		}

		return 0;
	}

	bool Cell::AddBezier(const Outline &o, const Outline &u, const uint32_t &i, const uint32_t &j,
	                     const uint32_t &contourIndex)
	{
		bool ret = true;
		uint32_t ucontourBegin = u.m_contours[contourIndex].begin;

		if (to != std::numeric_limits<uint32_t>::max() && to != j)
		{
			assert(to < j);

			if (from == ucontourBegin)
			{
				assert(to % 2 == 0);
				assert(from % 2 == 0);

				startLength = (to - from) / 2;
			}
			else
			{
				value = AddRange(value, from, to);

				if (!value)
				{
					ret = false;
				}
			}

			from = j;
		}
		else
		{
			if (from == std::numeric_limits<uint32_t>::max())
			{
				from = j;
			}
		}

		to = j + 2;
		return ret;
	}

	bool Cell::FinishContour(const Outline &o, const Outline &u, const uint32_t &contourIndex, uint32_t &maxStartLength)
	{
		bool ret = true;
		uint32_t ucontourBegin = u.m_contours[contourIndex].begin;
		uint32_t ucontourEnd = u.m_contours[contourIndex].end;

		if (to < ucontourEnd)
		{
			value = AddRange(value, from, to);

			if (!value)
			{
				ret = false;
			}

			from = std::numeric_limits<uint32_t>::max();
			to = std::numeric_limits<uint32_t>::max();
		}

		assert(to == std::numeric_limits<uint32_t>::max() || to == ucontourEnd);
		to = std::numeric_limits<uint32_t>::max();

		if (from != std::numeric_limits<uint32_t>::max() && startLength != 0)
		{
			value = AddRange(value, from, ucontourEnd + startLength * 2);

			if (!value)
			{
				ret = false;
			}

			maxStartLength = std::max(maxStartLength, startLength);
			from = std::numeric_limits<uint32_t>::max();
			startLength = 0;
		}

		if (from != std::numeric_limits<uint32_t>::max())
		{
			value = AddRange(value, from, ucontourEnd);

			if (!value)
			{
				ret = false;
			}

			from = std::numeric_limits<uint32_t>::max();
		}

		if (startLength != 0)
		{
			value = AddRange(value, ucontourBegin, ucontourBegin + startLength * 2);

			if (!value)
			{
				ret = false;
			}

			startLength = 0;
		}

		assert(from == std::numeric_limits<uint32_t>::max() && to == std::numeric_limits<uint32_t>::max());
		return ret;
	}

	bool Cell::CellsAddBezier(const Outline &o, const Outline &u, const uint32_t &i, const uint32_t &j,
	                          const uint32_t &contourIndex, Cell *cells)
	{
		Rect bezierBbox = bezier2_bbox(&o.m_points[i]);

		float outlineBboxW = o.m_bbox.maxX - o.m_bbox.minX;
		float outlineBboxH = o.m_bbox.maxY - o.m_bbox.minY;

		auto minX = static_cast<uint32_t>((bezierBbox.minX - o.m_bbox.minX) / outlineBboxW * o.m_cellCountX);
		auto minY = static_cast<uint32_t>((bezierBbox.minY - o.m_bbox.minY) / outlineBboxH * o.m_cellCountY);
		auto maxX = static_cast<uint32_t>((bezierBbox.maxX - o.m_bbox.minX) / outlineBboxW * o.m_cellCountX);
		auto maxY = static_cast<uint32_t>((bezierBbox.maxY - o.m_bbox.minY) / outlineBboxH * o.m_cellCountY);

		if (maxX >= o.m_cellCountX)
		{
			maxX = o.m_cellCountX - 1;
		}

		if (maxY >= o.m_cellCountY)
		{
			maxY = o.m_cellCountY - 1;
		}

		bool ret = true;

		for (uint32_t y = minY; y <= maxY; y++)
		{
			for (uint32_t x = minX; x <= maxX; x++)
			{
				Cell &cell = cells[y * o.m_cellCountX + x];

				if (bbox_bezier2_intersect(cell.bbox, &o.m_points[i]))
				{
					ret &= cell.AddBezier(o, u, i, j, contourIndex);
				}
			}
		}

		return ret;
	}

	bool Cell::CellsFinishContour(const Outline &o, const Outline &u, const uint32_t &contourIndex, Cell *cells,
	                              uint32_t &maxStartLength)
	{
		bool ret = true;

		for (uint32_t y = 0; y < o.m_cellCountY; y++)
		{
			for (uint32_t x = 0; x < o.m_cellCountX; x++)
			{
				Cell &cell = cells[y * o.m_cellCountX + x];
				ret &= cell.FinishContour(o, u, contourIndex, maxStartLength);
			}
		}

		return ret;
	}

	void Cell::CellsSetFilled(const Outline &u, Cell *cells, const uint32_t &filledCell)
	{
		for (uint32_t y = 0; y < u.m_cellCountY; y++)
		{
			for (uint32_t x = 0; x < u.m_cellCountX; x++)
			{
				uint32_t i = y * u.m_cellCountX + x;
				Cell *cell = &cells[i];

				if (cell->value == 0 && u.IsCellFilled(cell->bbox))
				{
					cell->value = filledCell;
				}
			}
		}
	}
}*/
