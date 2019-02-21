#include "Outline.hpp"

#include <cassert>
#include <vector>
#include <algorithm>
#include <Maths/Maths.hpp>

namespace acid
{
	void OutlineAddOddPoint(Outline *o)
	{
		if (o->points.size() % 2 != 0)
		{
			Vector2 p = Vector2(o->bbox.minX, o->bbox.minY);
			o->points.emplace_back(p);
		}
	}

	void ConvertPoint(const FT_Vector *v, Vector2 &out)
	{
		out.m_x = static_cast<float>(v->x) / 64.0f;
		out.m_y = static_cast<float>(v->y) / 64.0f;
	}

	int32_t MoveToFunc(const FT_Vector *to, Outline *o)
	{
		Vector2 p = Vector2();

		if (!o->contours.empty())
		{
			o->contours[o->contours.size() - 1].end = o->points.size() - 1;
			o->points.emplace_back(p);
		}

		assert(o->points.size() % 2 == 0);

		ContourRange range = {o->points.size(), std::numeric_limits<uint32_t>::max()};
		o->contours.emplace_back(range);

		ConvertPoint(to, p);
		o->points.emplace_back(p);
		return 0;
	}

	int32_t LineToFunc(const FT_Vector *to, Outline *o)
	{
		uint32_t last = o->points.size() - 1;

		Vector2 toP;
		ConvertPoint(to, toP);
		Vector2 p = o->points[last].Lerp(toP, 0.5f);
		o->points.emplace_back(p);
		o->points.emplace_back(toP);
		return 0;
	}

	int32_t ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o)
	{
		Vector2 p;
		ConvertPoint(control, p);
		o->points.emplace_back(p);

		ConvertPoint(to, p);
		o->points.emplace_back(p);
		return 0;
	}

	int32_t CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o)
	{
		return LineToFunc(to, o);
	}

	void OutlineConvert(FT_Outline *outline, Outline *o)
	{
		OutlineDecompose(outline, o);
	//	OutlineFixCorners(o);
	//	OutlineSubdivide(o);
		OutlineFixThinLines(o);
		OutlineMakeCells(o);
	}

	void OutlineDecompose(FT_Outline *outline, Outline *o)
	{
		memset(o, 0, sizeof(Outline)); // TODO: Remove

		FT_BBox outlineBbox;
		FT_CHECK(FT_Outline_Get_BBox(outline, &outlineBbox));

		o->bbox.minX = static_cast<float>(outlineBbox.xMin) / 64.0f;
		o->bbox.minY = static_cast<float>(outlineBbox.yMin) / 64.0f;
		o->bbox.maxX = static_cast<float>(outlineBbox.xMax) / 64.0f;
		o->bbox.maxY = static_cast<float>(outlineBbox.yMax) / 64.0f;

		FT_Outline_Funcs funcs = {};
		funcs.move_to = reinterpret_cast<FT_Outline_MoveToFunc>(MoveToFunc);
		funcs.line_to = reinterpret_cast<FT_Outline_LineToFunc>(LineToFunc);
		funcs.conic_to = reinterpret_cast<FT_Outline_ConicToFunc>(ConicToFunc);
		funcs.cubic_to = reinterpret_cast<FT_Outline_CubicToFunc>(CubicToFunc);

		FT_CHECK(FT_Outline_Decompose(outline, &funcs, o));

		if (!o->contours.empty())
		{
			o->contours[o->contours.size() - 1].end = o->points.size() - 1;
		}
	}

	uint32_t CellAddRange(uint32_t cell, uint32_t from, uint32_t to)
	{
		assert(from % 2 == 0 && to % 2 == 0);

		from /= 2;
		to /= 2;

		if (from >= std::numeric_limits<uint8_t>::max())
		{
			return 0;
		}

		if (to >= std::numeric_limits<uint8_t>::max())
		{
			return 0;
		}

		uint32_t length = to - from;

		if (length <= 3 && (cell & 0x03) == 0)
		{
			cell |= from << 8;
			cell |= length;
			return cell;
		}

		if (length > 7)
		{
			return 0;
		}

		if ((cell & 0x1C) == 0)
		{
			cell |= from << 16;
			cell |= length << 2;
			return cell;
		}

		if ((cell & 0xE0) == 0)
		{
			cell |= from << 24;
			cell |= length << 5;
			return cell;
		}

		return 0;
	}

	bool IsCellFilled(const Outline *o, const Rect &bbox)
	{
		// TODO: Optimize
		Vector2 p = Vector2(
			(bbox.maxX + bbox.minX) / 2.0f,
			(bbox.maxY + bbox.minY) / 2.0f
		);

		float mindist = std::numeric_limits<float>::max();
		float v = std::numeric_limits<float>::max();
		uint32_t j = std::numeric_limits<uint32_t>::max();

		for (uint32_t contourIndex = 0; contourIndex < o->contours.size(); contourIndex++)
		{
			uint32_t contourBegin = o->contours[contourIndex].begin;
			uint32_t contourEnd = o->contours[contourIndex].end;

			for (uint32_t i = contourBegin; i < contourEnd; i += 2)
			{
				Vector2 p0 = o->points[i];
				Vector2 p1 = o->points[i + 1];
				Vector2 p2 = o->points[i + 2];

				float t = LineCalculateT(p0, p2, p);

				Vector2 p02 = p0.Lerp(p2, t);

				float udist = p02.Distance(p);

				if (udist < mindist + 0.0001f)
				{
					float d = LineSignedDistance(p0, p2, p);

					if (udist >= mindist && i > contourBegin)
					{
						float lastD = i == contourEnd - 2 && j == contourBegin 
							? LineSignedDistance(p0, p2, o->points[contourBegin + 2])
							: LineSignedDistance(p0, p2, o->points[i - 2]);

						if (lastD < 0.0f)
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

	bool WipcellAddBezier(const Outline *o, const Outline *u, const uint32_t &i, const uint32_t &j, const uint32_t &contourIndex, WIPCell &cell)
	{
		bool ret = true;
		uint32_t ucontourBegin = u->contours[contourIndex].begin;

		if (cell.to != std::numeric_limits<uint32_t>::max() && cell.to != j)
		{
			assert(cell.to < j);

			if (cell.from == ucontourBegin)
			{
				assert(cell.to % 2 == 0);
				assert(cell.from % 2 == 0);

				cell.startLength = (cell.to - cell.from) / 2;
			}
			else
			{
				cell.value = CellAddRange(cell.value, cell.from, cell.to);

				if (!cell.value)
				{
					ret = false;
				}
			}

			cell.from = j;
		}
		else
		{
			if (cell.from == std::numeric_limits<uint32_t>::max())
			{
				cell.from = j;
			}
		}

		cell.to = j + 2;
		return ret;
	}

	bool WipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell &cell, uint32_t &maxStartLength)
	{
		bool ret = true;
		uint32_t ucontourBegin = u->contours[contourIndex].begin;
		uint32_t ucontourEnd = u->contours[contourIndex].end;

		if (cell.to < ucontourEnd)
		{
			cell.value = CellAddRange(cell.value, cell.from, cell.to);

			if (!cell.value)
			{
				ret = false;
			}

			cell.from = std::numeric_limits<uint32_t>::max();
			cell.to = std::numeric_limits<uint32_t>::max();
		}

		assert(cell.to == std::numeric_limits<uint32_t>::max() || cell.to == ucontourEnd);
		cell.to = std::numeric_limits<uint32_t>::max();

		if (cell.from != std::numeric_limits<uint32_t>::max() && cell.startLength != 0)
		{
			cell.value = CellAddRange(cell.value, cell.from, ucontourEnd + cell.startLength * 2);

			if (!cell.value)
			{
				ret = false;
			}

			maxStartLength = std::max(maxStartLength, cell.startLength);
			cell.from = std::numeric_limits<uint32_t>::max();
			cell.startLength = 0;
		}

		if (cell.from != std::numeric_limits<uint32_t>::max())
		{
			cell.value = CellAddRange(cell.value, cell.from, ucontourEnd);

			if (!cell.value)
			{
				ret = false;
			}

			cell.from = std::numeric_limits<uint32_t>::max();
		}

		if (cell.startLength != 0)
		{
			cell.value = CellAddRange(cell.value, ucontourBegin, ucontourBegin + cell.startLength * 2);

			if (!cell.value)
			{
				ret = false;
			}

			cell.startLength = 0;
		}

		assert(cell.from == std::numeric_limits<uint32_t>::max() && cell.to == std::numeric_limits<uint32_t>::max());
		return ret;
	}

	bool ForEachWipcellAddBezier(const Outline *o, const Outline *u, const uint32_t &i, const uint32_t &j, const uint32_t &contourIndex, WIPCell *cells)
	{
		Rect bezierBbox;
		Bezier2Bbox(&o->points[i], bezierBbox);

		float outlineBboxW = o->bbox.maxX - o->bbox.minX;
		float outlineBboxH = o->bbox.maxY - o->bbox.minY;

		auto minX = static_cast<uint32_t>((bezierBbox.minX - o->bbox.minX) / outlineBboxW * o->cellCountX);
		auto minY = static_cast<uint32_t>((bezierBbox.minY - o->bbox.minY) / outlineBboxH * o->cellCountY);
		auto maxX = static_cast<uint32_t>((bezierBbox.maxX - o->bbox.minX) / outlineBboxW * o->cellCountX);
		auto maxY = static_cast<uint32_t>((bezierBbox.maxY - o->bbox.minY) / outlineBboxH * o->cellCountY);

		if (maxX >= o->cellCountX)
		{
			maxX = o->cellCountX - 1;
		}

		if (maxY >= o->cellCountY)
		{
			maxY = o->cellCountY - 1;
		}

		bool ret = true;

		for (uint32_t y = minY; y <= maxY; y++)
		{
			for (uint32_t x = minX; x <= maxX; x++)
			{
				WIPCell *cell = &cells[y * o->cellCountX + x];

				if (BboxBezier2Intersect(cell->bbox, &o->points[i]))
				{
					ret &= WipcellAddBezier(o, u, i, j, contourIndex, *cell);
				}
			}
		}

		return ret;
	}

	bool ForEachWipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell *cells,
		uint32_t &maxStartLength)
	{
		bool ret = true;

		for (uint32_t y = 0; y < o->cellCountY; y++)
		{
			for (uint32_t x = 0; x < o->cellCountX; x++)
			{
				WIPCell *cell = &cells[y * o->cellCountX + x];
				ret &= WipcellFinishContour(o, u, contourIndex, *cell, maxStartLength);
			}
		}

		return ret;
	}

	void CopyWipcellValues(Outline *u, const WIPCell *cells)
	{
		u->cells = std::vector<uint32_t>(u->cellCountX * u->cellCountY);

		for (uint32_t y = 0; y < u->cellCountY; y++)
		{
			for (uint32_t x = 0; x < u->cellCountX; x++)
			{
				uint32_t i = y * u->cellCountX + x;
				u->cells[i] = cells[i].value;
			}
		}
	}

	void InitWipcells(const Outline *o, WIPCell *cells)
	{
		float w = o->bbox.maxX - o->bbox.minX;
		float h = o->bbox.maxY - o->bbox.minY;

		for (uint32_t y = 0; y < o->cellCountY; y++)
		{
			for (uint32_t x = 0; x < o->cellCountX; x++)
			{
				Rect bbox = {
					o->bbox.minX + (static_cast<float>(x) / o->cellCountX) * w,
					o->bbox.minY + (static_cast<float>(y) / o->cellCountY) * h,
					o->bbox.minX + (static_cast<float>(x + 1) / o->cellCountX) * w,
					o->bbox.minY + (static_cast<float>(y + 1) / o->cellCountY) * h,
				};

				uint32_t i = y * o->cellCountX + x;
				cells[i].bbox = bbox;
				cells[i].from = std::numeric_limits<uint32_t>::max();
				cells[i].to = std::numeric_limits<uint32_t>::max();
				cells[i].value = 0;
				cells[i].startLength = 0;
			}
		}
	}

	uint32_t OutlineAddFilledLine(Outline *o)
	{
		OutlineAddOddPoint(o);

		uint32_t i = o->points.size();
		float y = o->bbox.maxY + 1000.0f;
		Vector2 f0 = Vector2(o->bbox.minX, y);
		Vector2 f1 = Vector2(o->bbox.minX + 10.0f, y);
		Vector2 f2 = Vector2(o->bbox.minX + 20.0f, y);
		o->points.emplace_back(f0);
		o->points.emplace_back(f1);
		o->points.emplace_back(f2);

		return i;
	}

	uint32_t MakeCellFromSingleEdge(const uint32_t &e)
	{
		assert(e % 2 == 0);
		return e << 7 | 1;
	}

	void SetFilledCells(const Outline *u, WIPCell *cells, const uint32_t &filledCell)
	{
		for (uint32_t y = 0; y < u->cellCountY; y++)
		{
			for (uint32_t x = 0; x < u->cellCountX; x++)
			{
				uint32_t i = y * u->cellCountX + x;
				WIPCell *cell = &cells[i];

				if (cell->value == 0 && IsCellFilled(u, cell->bbox))
				{
					cell->value = filledCell;
				}
			}
		}
	}

	bool TryToFitInCellCount(Outline *o)
	{
		bool ret = true;

		auto cells = std::vector<WIPCell>(o->cellCountX * o->cellCountY);
		InitWipcells(o, cells.data());

		Outline u = {};
		u.bbox = o->bbox;
		u.cellCountX = o->cellCountX;
		u.cellCountY = o->cellCountY;

		for (uint32_t contourIndex = 0; contourIndex < o->contours.size(); contourIndex++)
		{
			uint32_t contourBegin = o->contours[contourIndex].begin;
			uint32_t contourEnd = o->contours[contourIndex].end;

			OutlineAddOddPoint(&u);

			ContourRange urange = {u.points.size(), u.points.size() + contourEnd - contourBegin};
			u.contours.emplace_back(urange);

			for (uint32_t i = contourBegin; i < contourEnd; i += 2)
			{
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];
			//	Vector2 &p2 = o->points[i + 2];

				uint32_t j = u.points.size();
				u.points.emplace_back(p0);
				u.points.emplace_back(p1);

				ret &= ForEachWipcellAddBezier(o, &u, i, j, contourIndex, cells.data());
			}

			uint32_t max_start_len = 0;
			ret &= ForEachWipcellFinishContour(o, &u, contourIndex, cells.data(), max_start_len);

			uint32_t continuationEnd = contourBegin + max_start_len * 2;

			for (uint32_t i = contourBegin; i < continuationEnd; i += 2)
			{
				u.points.emplace_back(o->points[i]);
				u.points.emplace_back(o->points[i + 1]);
			}

			Vector2 &plast = o->points[continuationEnd];
			u.points.emplace_back(plast);
		}

		if (!ret)
		{
			return ret;
		}

		uint32_t filledLine = OutlineAddFilledLine(&u);
		uint32_t filledCell = MakeCellFromSingleEdge(filledLine);
		SetFilledCells(&u, cells.data(), filledCell);

		CopyWipcellValues(&u, cells.data());

		*o = u;
		return ret;
	}

	uint32_t Uint32ToPow2(uint32_t v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		return v;
	}

	void OutlineMakeCells(Outline *o)
	{
		if (o->points.size() > FD_OUTLINE_MAX_POINTS)
		{
			return;
		}

		float w = o->bbox.maxX - o->bbox.minX;
		float h = o->bbox.maxY - o->bbox.minY;

		float multiplier = 0.5f;

		if (h > w * 1.8f || w > h * 1.8f)
		{
			multiplier = 1.0f;
		}

		uint32_t c = Uint32ToPow2(static_cast<uint32_t>(std::sqrt(o->points.size() * 0.75f)));

		o->cellCountX = c;
		o->cellCountY = c;

		if (h > w * 1.8f)
		{
			o->cellCountX /= 2;
		}

		if (w > h * 1.8f)
		{
			o->cellCountY /= 2;
		}

		while (true)
		{
			if (TryToFitInCellCount(o))
			{
				break;
			}

			if (o->cellCountX > 64 || o->cellCountY > 64)
			{
				o->cellCountX = 0;
				o->cellCountY = 0;
				return;
			}

			if (o->cellCountX == o->cellCountY)
			{
				if (w > h)
				{
					o->cellCountX *= 2;
				}
				else
				{
					o->cellCountY *= 2;
				}
			}
			else
			{
				if (o->cellCountX < o->cellCountY)
				{
					o->cellCountX *= 2;
				}
				else
				{
					o->cellCountY *= 2;
				}
			}
		}
	}

	void OutlineSubdivide(Outline *o)
	{
		Outline u = {};
		u.bbox = o->bbox;

		for (uint32_t contourIndex = 0; contourIndex < o->contours.size(); contourIndex++)
		{
			uint32_t contourBegin = o->contours[contourIndex].begin;
			uint32_t contour_end = o->contours[contourIndex].end;

			OutlineAddOddPoint(&u);

			ContourRange urange = {u.points.size(), std::numeric_limits<uint32_t>::max()};
			u.contours.emplace_back(urange);

			for (uint32_t i = contourBegin; i < contour_end; i += 2)
			{
				Vector2 &p0 = o->points[i];
			//	Vector2 &p1 = o->points[i + 1];
			//	Vector2 &p2 = o->points[i + 2];

				Vector2 newp[3];
				Bezier2Split_3P(newp, &o->points[i], 0.5f);

				u.points.emplace_back(p0);
				u.points.emplace_back(newp[0]);
				u.points.emplace_back(newp[1]);
				u.points.emplace_back(newp[2]);
			}

			u.contours[contourIndex].end = u.points.size();
			u.points.emplace_back(o->points[contour_end]);
		}

		*o = u;
	}

	void OutlineFixCorners(Outline *o)
	{
		float fixDist = 0.001f;

		for (uint32_t contourIndex = 0; contourIndex < o->contours.size(); contourIndex++)
		{
			uint32_t contourBegin = o->contours[contourIndex].begin;
			uint32_t contourEnd = o->contours[contourIndex].end;

			for (uint32_t i = contourBegin; i < contourEnd; i += 2)
			{
				uint32_t prev = i - 1;

				if (contourBegin == i)
				{
					prev = contourEnd - 1;
				}

				Vector2 &r = o->points[prev];
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];

				Vector2 v0 = r - p0;
				Vector2 v1 = p1 - p0;

				v0 = v0.Normalize();
				v1 = v1.Normalize();

				float angle = std::acos(v0.Dot(v1));

				if (angle <= Maths::Pi / 2.0f * 1.025f)
				{
					v0 *= fixDist;
					v1 *= fixDist;

					Vector2 f1 = p0 - v0;
					Vector2 f0 = p0 - v1;

					OutlineAddOddPoint(o);

					if (o->cornerFixBegin == 0)
					{
						o->cornerFixBegin = o->points.size();
					}

					o->points.emplace_back(f0);
					o->points.emplace_back(f1);
				}
			}
		}
	}

	void OutlineFixThinLines(Outline *o)
	{
		// TODO: Optimize
		Outline u = {};
		u.bbox = o->bbox;

		for (uint32_t contourIndex = 0; contourIndex < o->contours.size(); contourIndex++)
		{
			uint32_t contourBegin = o->contours[contourIndex].begin;
			uint32_t contourEnd = o->contours[contourIndex].end;

			OutlineAddOddPoint(&u);

			ContourRange urange = {u.points.size(), std::numeric_limits<uint32_t>::max()};
			u.contours.emplace_back(urange);

			for (uint32_t i = contourBegin; i < contourEnd; i += 2)
			{
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];
				Vector2 &p2 = o->points[i + 2];

				Vector2 mid = p0.Lerp(p2, 0.5f);
				Vector2 midp1 = p1 - mid;

				Vector2 bezier[] = {
					{p0.m_x, p0.m_y},
					{p1.m_x, p1.m_y},
					{p2.m_x, p2.m_y}
				};

				bezier[1] += midp1;
				/*bool subdivide = false;

				if (i > 2)
				{
					uint32_t jbegin = contourBegin;

					if (i == contourEnd - 2)
					{
						jbegin += 2;
					}

					for (uint32_t j = jbegin; j < i - 2; j += 2)
					{
						Vector2 &q0 = o->points[j];
						Vector2 &q2 = o->points[j + 2];

						if (Bezier2LineIsIntersecting(bezier, q0, q2))
						{
							subdivide = true;
						}
					}
				}

				uint32_t jend = contourEnd;

				if (i == contourBegin)
				{
					jend -= 2;
				}

				for (uint32_t j = i + 2; j < jend; j += 2)
				{
					Vector2 &q0 = o->points[j];
					Vector2 &q2 = o->points[j + 2];

					if (Bezier2LineIsIntersecting(bezier, q0, q2))
					{
						subdivide = true;
					}
				}*/

				bool subdivide = false;

				for (uint32_t j = contourBegin; j < contourEnd; j += 2)
				{
					if (i == contourBegin && j == contourEnd - 2)
					{
						continue;
					}

					if (i == contourEnd - 2 && j == contourBegin)
					{
						continue;
					}

					if (j + 2 >= i && j <= i + 2)
					{
						continue;
					}

					Vector2 &q0 = o->points[j];
				//	Vector2 &q1 = o->points[j + 1];
					Vector2 &q2 = o->points[j + 2];

					if (Bezier2LineIsIntersecting(bezier, q0, q2))
					{
						subdivide = true;
					}
				}

				if (subdivide)
				{
					Vector2 newp[3];
					Bezier2Split_3P(newp, &o->points[i], 0.5f);

					u.points.emplace_back(p0);
					u.points.emplace_back(newp[0]);
					u.points.emplace_back(newp[1]);
					u.points.emplace_back(newp[2]);
				}
				else
				{
					u.points.emplace_back(p0);
					u.points.emplace_back(p1);
				}
			}

			u.contours[contourIndex].end = u.points.size();
			u.points.emplace_back(o->points[contourEnd]);
		}

		*o = u;
	}

	void OutlineCbox(Outline *o, Rect *cbox)
	{
		if (o->points.empty())
		{
			return;
		}

		cbox->minX = o->points[0].m_x;
		cbox->minY = o->points[0].m_y;
		cbox->maxX = o->points[0].m_x;
		cbox->maxY = o->points[0].m_y;

		for (const auto &point : o->points)
		{
			cbox->minX = std::min(cbox->minX, point.m_x);
			cbox->minY = std::min(cbox->minY, point.m_y);
			cbox->maxX = std::max(cbox->maxX, point.m_x);
			cbox->maxY = std::max(cbox->maxY, point.m_y);
		}
	}

	uint16_t GenU16Value(const float &x, const float &min, const float &max)
	{
		return static_cast<uint16_t>((x - min) / (max - min) * std::numeric_limits<uint16_t>::max());
	}

	void OutlineU16Points(Outline *o, Rect *cbox, PointU16 *pout)
	{
		OutlineCbox(o, cbox);

		for (uint32_t i = 0; i < o->points.size(); i++)
		{
			float x = o->points[i].m_x;
			float y = o->points[i].m_y;

			pout[i].x = GenU16Value(x, cbox->minX, cbox->maxX);
			pout[i].y = GenU16Value(y, cbox->minY, cbox->maxY);
		}
	}
}
