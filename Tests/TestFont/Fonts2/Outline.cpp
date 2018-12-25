/*#include "Outline.hpp"

#include <assert.h>
#include <string.h>
#include <vector>
#include "Maths/Maths.hpp"
#include "Geometry.hpp"
#include "Cell.hpp"

namespace acid
{
	void Outline::AddPoint(const Vector2 &point)
	{
		if (m_pointCapacity == m_pointCount)
		{
			dyn_array_grow((void **) &m_points, &m_pointCapacity, sizeof(Vector2));
		}

		m_points[m_pointCount] = point;
		m_pointCount++;
	}

	void Outline::AddContour(const ContourRange &range)
	{
		if (m_contourCapacity == m_contourCount)
		{
			dyn_array_grow(&m_contours, &m_contourCapacity, sizeof(ContourRange));
		}

		m_contours[m_contourCount] = range;
		m_contourCount++;
	}

	void Outline::AddOddPoint()
	{
		if (m_pointCount % 2 != 0)
		{
			Vector2 p = {m_bbox.minX, m_bbox.minY};
			AddPoint(p);
		}
	}

	static Vector2 convert_point(const FT_Vector *v)
	{
		return Vector2(
			static_cast<float>(v->x) / 64.0f,
			static_cast<float>(v->y) / 64.0f
		);
	}

	static int move_to_func(const FT_Vector *to, Outline &o)
	{
		Vector2 p = Vector2();

		if (o.m_contourCount > 0)
		{
			o.m_contours[o.m_contourCount - 1].end = o.m_pointCount - 1;
			o.AddPoint(p);
		}

		assert(o.m_pointCount % 2 == 0);

		ContourRange range = {o.m_pointCount, std::numeric_limits<uint32_t>::max()};
		o.AddContour(range);

		p = convert_point(to);
		o.AddPoint(p);
		return 0;
	}

	static int line_to_func(const FT_Vector *to, Outline &o)
	{
		uint32_t last = o.m_pointCount - 1;

		Vector2 to_p;
		to_p = convert_point(to);
		Vector2 p = o.m_points[last].Lerp(to_p, 0.5f);
		o.AddPoint(p);
		o.AddPoint(to_p);
		return 0;
	}

	static int conic_to_func(const FT_Vector *control, const FT_Vector *to, Outline &o)
	{
		Vector2 p;
		p = convert_point(control);
		o.AddPoint(p);

		p = convert_point(to);
		o.AddPoint(p);
		return 0;
	}

	static int cubic_to_func(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline &o)
	{
		return line_to_func(to, o);
	}

	void Outline::Decompose(FT_Outline *outline)
	{
		memset(this, 0, sizeof(Outline));

		FT_BBox outline_bbox;
		FT_CHECK(FT_Outline_Get_BBox(outline, &outline_bbox));

		m_bbox.minX = static_cast<float>(outline_bbox.xMin) / 64.0f;
		m_bbox.minY = static_cast<float>(outline_bbox.yMin) / 64.0f;
		m_bbox.maxX = static_cast<float>(outline_bbox.xMax) / 64.0f;
		m_bbox.maxY = static_cast<float>(outline_bbox.yMax) / 64.0f;

		FT_Outline_Funcs funcs = {};
		funcs.move_to = reinterpret_cast<FT_Outline_MoveToFunc>(move_to_func);
		funcs.line_to = reinterpret_cast<FT_Outline_LineToFunc>(line_to_func);
		funcs.conic_to = reinterpret_cast<FT_Outline_ConicToFunc>(conic_to_func);
		funcs.cubic_to = reinterpret_cast<FT_Outline_CubicToFunc>(cubic_to_func);

		FT_CHECK(FT_Outline_Decompose(outline, &funcs, this));

		if (m_contourCount > 0)
		{
			m_contours[m_contourCount - 1].end = m_pointCount - 1;
		}
	}

	bool Outline::IsCellFilled(const Rect &bbox) const
	{
		Vector2 p = {
			(bbox.maxX + bbox.minX) / 2.0f,
			(bbox.maxY + bbox.minY) / 2.0f,
		};

		float mindist = std::numeric_limits<float>::max();
		float v = std::numeric_limits<float>::max();
		uint32_t j = std::numeric_limits<uint32_t>::max();

		for (uint32_t contour_index = 0; contour_index < m_contourCount; contour_index++)
		{
			uint32_t contour_begin = m_contours[contour_index].begin;
			uint32_t contour_end = m_contours[contour_index].end;

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 p0 = m_points[i];
				Vector2 p1 = m_points[i + 1];
				Vector2 p2 = m_points[i + 2];

				float t = line_calculate_t(p0, p2, p);

				Vector2 p02 = p0.Lerp(p2, t);

				float udist = p02.Distance(p);

				if (udist < mindist + 0.0001f)
				{
					float d = line_signed_distance(p0, p2, p);

					if (udist >= mindist && i > contour_begin)
					{
						float lastd = i == contour_end - 2 && j == contour_begin
						    ? line_signed_distance(p0, p2, m_points[contour_begin + 2])
						    : line_signed_distance(p0, p2, m_points[i - 2]);

						if (lastd < 0.0f)
						{
							v = std::max(d, v);
						}
						else
						{
							v = std::min(d, v);
						}
					}
					else
					{
						v = d;
					}

					mindist = std::min(mindist, udist);
					j = i;
				}
			}
		}

		return v > 0.0f;
	}

	void Outline::CopyCellValues(Cell *wipCells)
	{
		m_cells = (uint32_t *) malloc(sizeof(uint32_t) * m_cellCountX * m_cellCountY);

		for (uint32_t y = 0; y < m_cellCountY; y++)
		{
			for (uint32_t x = 0; x < m_cellCountX; x++)
			{
				uint32_t i = y * m_cellCountX + x;
				m_cells[i] = wipCells[i].value;
			}
		}
	}

	void Outline::InitCells(Cell *wipCells)
	{
		float w = m_bbox.maxX - m_bbox.minX;
		float h = m_bbox.maxY - m_bbox.minY;

		for (uint32_t y = 0; y < m_cellCountY; y++)
		{
			for (uint32_t x = 0; x < m_cellCountX; x++)
			{
				Rect cbox = {
					m_bbox.minX + ((float) x / m_cellCountX) * w,
					m_bbox.minY + ((float) y / m_cellCountY) * h,
					m_bbox.minX + ((float) (x + 1) / m_cellCountX) * w,
					m_bbox.minY + ((float) (y + 1) / m_cellCountY) * h,
				};

				uint32_t i = y * m_cellCountX + x;
				wipCells[i].bbox = cbox;
				wipCells[i].from = std::numeric_limits<uint32_t>::max();
				wipCells[i].to = std::numeric_limits<uint32_t>::max();
				wipCells[i].value = 0;
				wipCells[i].startLength = 0;
			}
		}
	}

	uint32_t Outline::AddFilledLine()
	{
		AddOddPoint();

		uint32_t i = m_pointCount;
		float y = m_bbox.maxY + 1000.0f;
		Vector2 f0 = {m_bbox.minX, y};
		Vector2 f1 = {m_bbox.minX + 10.0f, y};
		Vector2 f2 = {m_bbox.minX + 20.0f, y};
		AddPoint(f0);
		AddPoint(f1);
		AddPoint(f2);

		return i;
	}

	static uint32_t make_cell_from_single_edge(const uint32_t &e)
	{
		assert(e % 2 == 0);
		return e << 7 | 1;
	}

	bool Outline::TryFitCellCount()
	{
		bool ret = true;

		auto cells = std::vector<Cell>(m_cellCountX * m_cellCountY);
		InitCells(cells.data());

		Outline u = {};
		u.m_bbox = m_bbox;
		u.m_cellCountX = m_cellCountX;
		u.m_cellCountY = m_cellCountY;

		for (uint32_t contour_index = 0; contour_index < m_contourCount; contour_index++)
		{
			uint32_t contour_begin = m_contours[contour_index].begin;
			uint32_t contour_end = m_contours[contour_index].end;

			u.AddOddPoint();

			ContourRange urange = {u.m_pointCount, u.m_pointCount + contour_end - contour_begin};
			u.AddContour(urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 p0 = m_points[i];
				Vector2 p1 = m_points[i + 1];

				uint32_t j = u.m_pointCount;
				u.AddPoint(p0);
				u.AddPoint(p1);

				ret &= Cell::CellsAddBezier(*this, u, i, j, contour_index, cells.data());
			}

			uint32_t maxStartLength = 0;
			ret &= Cell::CellsFinishContour(*this, u, contour_index, cells.data(), maxStartLength);

			uint32_t continuation_end = contour_begin + maxStartLength * 2;

			for (uint32_t i = contour_begin; i < continuation_end; i += 2)
			{
				u.AddPoint(m_points[i]);
				u.AddPoint(m_points[i + 1]);
			}

			Vector2 plast = m_points[continuation_end];
			u.AddPoint(plast);
		}

		if (!ret)
		{
			u.Destroy();
			return ret;
		}

		uint32_t filled_line = u.AddFilledLine();
		uint32_t filled_cell = make_cell_from_single_edge(filled_line);
		Cell::CellsSetFilled(u, cells.data(), filled_cell);

		u.CopyCellValues(cells.data());

		Destroy();
		*this = u;
		return ret;
	}

	static uint32_t uint32_to_pow2(const uint32_t &v)
	{
		uint32_t r = v;
		r--;
		r |= r >> 1;
		r |= r >> 2;
		r |= r >> 4;
		r |= r >> 8;
		r |= r >> 16;
		r++;
		return r;
	}

	void Outline::MakeCells()
	{
		if (m_pointCount > OUTLINE_MAX_POINTS)
		{
			return;
		}

		float w = m_bbox.maxX - m_bbox.minX;
		float h = m_bbox.maxY - m_bbox.minY;

		uint32_t c = uint32_to_pow2((uint32_t) std::sqrt(m_pointCount * 0.75f));

		m_cellCountX = c;
		m_cellCountY = c;

		if (h > w * 1.8f)
		{
			m_cellCountX /= 2;
		}

		if (w > h * 1.8f)
		{
			m_cellCountY /= 2;
		}

		while (true)
		{
			if (TryFitCellCount())
			{
				break;
			}

			if (m_cellCountX > 64 || m_cellCountY > 64)
			{
				m_cellCountX = 0;
				m_cellCountY = 0;
				return;
			}

			if (m_cellCountX == m_cellCountY)
			{
				if (w > h)
				{
					m_cellCountX *= 2;
				}
				else
				{
					m_cellCountY *= 2;
				}
			}
			else
			{
				if (m_cellCountX < m_cellCountY)
				{
					m_cellCountX *= 2;
				}
				else
				{
					m_cellCountY *= 2;
				}
			}
		}
	}

	void Outline::Subdivide()
	{
		Outline u = {};
		u.m_bbox = m_bbox;

		for (uint32_t contour_index = 0; contour_index < m_contourCount; contour_index++)
		{
			uint32_t contour_begin = m_contours[contour_index].begin;
			uint32_t contour_end = m_contours[contour_index].end;

			u.AddOddPoint();

			ContourRange urange = {u.m_pointCount, std::numeric_limits<uint32_t>::max()};
			u.AddContour(urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 p0 = m_points[i];

				Vector2 newp[3];
				bezier2_split_3p(newp, &m_points[i], 0.5f);

				u.AddPoint(p0);
				u.AddPoint(newp[0]);
				u.AddPoint(newp[1]);
				u.AddPoint(newp[2]);
			}

			u.m_contours[contour_index].end = u.m_pointCount;
			u.AddPoint(m_points[contour_end]);
		}

		Destroy();
		*this = u;
	}

	void Outline::FixThinLines()
	{
		Outline u = {};
		u.m_bbox = m_bbox;

		for (uint32_t contour_index = 0; contour_index < m_contourCount; contour_index++)
		{
			uint32_t contour_begin = m_contours[contour_index].begin;
			uint32_t contour_end = m_contours[contour_index].end;

			u.AddOddPoint();

			ContourRange urange = {u.m_pointCount, std::numeric_limits<uint32_t>::max()};
			u.AddContour(urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 p0 = m_points[i];
				Vector2 p1 = m_points[i + 1];
				Vector2 p2 = m_points[i + 2];

				Vector2 mid = p0.Lerp(p2, 0.5f);
				Vector2 midp1 = p1 - mid;

				Vector2 bezier[] = {
					{p0.m_x, p0.m_y},
					{p1.m_x, p1.m_y},
					{p2.m_x, p2.m_y}
				};

				bezier[1] += midp1;
				bool subdivide = false;

				for (uint32_t j = contour_begin; j < contour_end; j += 2)
				{
					if (i == contour_begin && j == contour_end - 2)
					{
						continue;
					}

					if (i == contour_end - 2 && j == contour_begin)
					{
						continue;
					}

					if (j + 2 >= i && j <= i + 2)
					{
						continue;
					}

					Vector2 q0 = m_points[j];
					Vector2 q2 = m_points[j + 2];

					if (bezier2_line_is_intersecting(bezier, q0, q2))
					{
						subdivide = true;
					}
				}

				if (subdivide)
				{
					Vector2 newp[3];
					bezier2_split_3p(newp, &m_points[i], 0.5f);

					u.AddPoint(p0);
					u.AddPoint(newp[0]);
					u.AddPoint(newp[1]);
					u.AddPoint(newp[2]);
				}
				else
				{
					u.AddPoint(p0);
					u.AddPoint(p1);
				}
			}

			u.m_contours[contour_index].end = u.m_pointCount;
			u.AddPoint(m_points[contour_end]);
		}

		Destroy();
		*this = u;
	}

	void Outline::Convert(FT_Outline *outline)
	{
		Decompose(outline);
		FixThinLines();
		MakeCells();
	}

	void Outline::Destroy()
	{
		if (m_contours)
		{
			free(m_contours);
		}

		if (m_points)
		{
			free(m_points);
		}

		if (m_cells)
		{
			free(m_cells);
		}

		memset(this, 0, sizeof(Outline));
	}

	Rect Outline::GetCbox() const
	{
		if (m_pointCount == 0)
		{
			return {};
		}

		Rect cbox = {};
		cbox.minX = m_points[0].m_x;
		cbox.minY = m_points[0].m_y;
		cbox.maxX = m_points[0].m_x;
		cbox.maxY = m_points[0].m_y;

		for (uint32_t i = 1; i < m_pointCount; i++)
		{
			float x = m_points[i].m_x;
			float y = m_points[i].m_y;

			cbox.minX = std::min(cbox.minX, x);
			cbox.minY = std::min(cbox.minY, y);
			cbox.maxX = std::max(cbox.maxX, x);
			cbox.maxY = std::max(cbox.maxY, y);
		}

		return cbox;
	}

	static uint16_t gen_u16_value(const float &x, const float &min, const float &max)
	{
		return static_cast<uint16_t>((x - min) / (max - min) * std::numeric_limits<uint16_t>::max());
	}
}*/
