#include "Outline.hpp"

#include <ft2build.h>
#include FT_BBOX_H
#include FT_OUTLINE_H
#include "Maths/Maths.hpp"

namespace acid
{
static const uint32_t FD_OUTLINE_MAX_POINTS = 255 * 2;

void OutlineAddOddPoint(Outline *o)
{
	if (o->m_points.size() % 2 != 0)
	{
		Vector2f p = Vector2f(o->m_bbox.m_min.m_x, o->m_bbox.m_min.m_y);
		o->m_points.emplace_back(p);
	}
}

void ConvertPoint(const FT_Vector *v, Vector2f &out)
{
	out.m_x = static_cast<float>(v->x) / 64.0f;
	out.m_y = static_cast<float>(v->y) / 64.0f;
}

int32_t MoveToFunc(const FT_Vector *to, Outline *o)
{
	Vector2f p;

	if (!o->m_contours.empty())
	{
		o->m_contours[o->m_contours.size() - 1].m_end = static_cast<uint32_t>(o->m_points.size() - 1);
		o->m_points.emplace_back(p);
	}

	assert(o->m_points.size() % 2 == 0);

	ContourRange range = { static_cast<uint32_t>(o->m_points.size()), std::numeric_limits<uint32_t>::max() };
	o->m_contours.emplace_back(range);

	ConvertPoint(to, p);
	o->m_points.emplace_back(p);
	return 0;
}

int32_t LineToFunc(const FT_Vector *to, Outline *o)
{
	auto last = static_cast<uint32_t>(o->m_points.size() - 1);

	Vector2f toP;
	ConvertPoint(to, toP);
	Vector2f p = o->m_points[last].Lerp(toP, 0.5f);
	o->m_points.emplace_back(p);
	o->m_points.emplace_back(toP);
	return 0;
}

int32_t ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o)
{
	Vector2f p;
	ConvertPoint(control, p);
	o->m_points.emplace_back(p);

	ConvertPoint(to, p);
	o->m_points.emplace_back(p);
	return 0;
}

int32_t CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o)
{
	return LineToFunc(to, o);
}

void OutlineConvert(FT_Outline *outline, Outline *o)
{
	OutlineDecompose(outline, o);
	//OutlineFixCorners(o);
	//OutlineSubdivide(o);
	OutlineFixThinLines(o);
	OutlineMakeCells(o);
}

void OutlineDecompose(FT_Outline *outline, Outline *o)
{
	//memset(o, 0, sizeof(Outline)); // TODO: Remove
	*o = {};

	FT_BBox outlineBbox;

	if (FT_Outline_Get_BBox(outline, &outlineBbox) != 0)
	{
		throw std::runtime_error("Freetype failed to get a bbox outline");
	}

	o->m_bbox.m_min.m_x = static_cast<float>(outlineBbox.xMin) / 64.0f;
	o->m_bbox.m_min.m_y = static_cast<float>(outlineBbox.yMin) / 64.0f;
	o->m_bbox.m_max.m_x = static_cast<float>(outlineBbox.xMax) / 64.0f;
	o->m_bbox.m_max.m_y = static_cast<float>(outlineBbox.yMax) / 64.0f;

	FT_Outline_Funcs funcs = {};
	funcs.move_to = reinterpret_cast<FT_Outline_MoveToFunc>(MoveToFunc);
	funcs.line_to = reinterpret_cast<FT_Outline_LineToFunc>(LineToFunc);
	funcs.conic_to = reinterpret_cast<FT_Outline_ConicToFunc>(ConicToFunc);
	funcs.cubic_to = reinterpret_cast<FT_Outline_CubicToFunc>(CubicToFunc);

	if (FT_Outline_Decompose(outline, &funcs, o) != 0)
	{
		throw std::runtime_error("Freetype failed to decompose a outline");
	}

	if (!o->m_contours.empty())
	{
		o->m_contours[o->m_contours.size() - 1].m_end = static_cast<uint32_t>(o->m_points.size() - 1);
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
	Vector2f p = Vector2f((bbox.m_max.m_x + bbox.m_min.m_x) / 2.0f, (bbox.m_max.m_y + bbox.m_min.m_y) / 2.0f);

	float mindist = std::numeric_limits<float>::max();
	float v = std::numeric_limits<float>::max();
	uint32_t j = std::numeric_limits<uint32_t>::max();

	for (uint32_t contourIndex = 0; contourIndex < o->m_contours.size(); contourIndex++)
	{
		uint32_t contourBegin = o->m_contours[contourIndex].m_begin;
		uint32_t contourEnd = o->m_contours[contourIndex].m_end;

		for (uint32_t i = contourBegin; i < contourEnd; i += 2)
		{
			Vector2f p0 = o->m_points[i];
			Vector2f p1 = o->m_points[i + 1];
			Vector2f p2 = o->m_points[i + 2];

			float t = LineCalculateT(p0, p2, p);

			Vector2f p02 = p0.Lerp(p2, t);

			float udist = p02.Distance(p);

			if (udist < mindist + 0.0001f)
			{
				float d = LineSignedDistance(p0, p2, p);

				if (udist >= mindist && i > contourBegin)
				{
					float lastD = i == contourEnd - 2 && j == contourBegin ? LineSignedDistance(p0, p2, o->m_points[contourBegin + 2]) : LineSignedDistance(p0, p2, o->m_points[i - 2]);

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
	uint32_t ucontourBegin = u->m_contours[contourIndex].m_begin;

	if (cell.m_to != std::numeric_limits<uint32_t>::max() && cell.m_to != j)
	{
		assert(cell.m_to < j);

		if (cell.m_from == ucontourBegin)
		{
			assert(cell.m_to % 2 == 0);
			assert(cell.m_from % 2 == 0);

			cell.m_startLength = (cell.m_to - cell.m_from) / 2;
		}
		else
		{
			cell.m_value = CellAddRange(cell.m_value, cell.m_from, cell.m_to);

			if (!cell.m_value)
			{
				ret = false;
			}
		}

		cell.m_from = j;
	}
	else
	{
		if (cell.m_from == std::numeric_limits<uint32_t>::max())
		{
			cell.m_from = j;
		}
	}

	cell.m_to = j + 2;
	return ret;
}

bool WipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell &cell, uint32_t &maxStartLength)
{
	bool ret = true;
	uint32_t ucontourBegin = u->m_contours[contourIndex].m_begin;
	uint32_t ucontourEnd = u->m_contours[contourIndex].m_end;

	if (cell.m_to < ucontourEnd)
	{
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, cell.m_to);

		if (!cell.m_value)
		{
			ret = false;
		}

		cell.m_from = std::numeric_limits<uint32_t>::max();
		cell.m_to = std::numeric_limits<uint32_t>::max();
	}

	assert(cell.m_to == std::numeric_limits<uint32_t>::max() || cell.m_to == ucontourEnd);
	cell.m_to = std::numeric_limits<uint32_t>::max();

	if (cell.m_from != std::numeric_limits<uint32_t>::max() && cell.m_startLength != 0)
	{
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, ucontourEnd + cell.m_startLength * 2);

		if (!cell.m_value)
		{
			ret = false;
		}

		maxStartLength = std::max(maxStartLength, cell.m_startLength);
		cell.m_from = std::numeric_limits<uint32_t>::max();
		cell.m_startLength = 0;
	}

	if (cell.m_from != std::numeric_limits<uint32_t>::max())
	{
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, ucontourEnd);

		if (!cell.m_value)
		{
			ret = false;
		}

		cell.m_from = std::numeric_limits<uint32_t>::max();
	}

	if (cell.m_startLength != 0)
	{
		cell.m_value = CellAddRange(cell.m_value, ucontourBegin, ucontourBegin + cell.m_startLength * 2);

		if (!cell.m_value)
		{
			ret = false;
		}

		cell.m_startLength = 0;
	}

	assert(cell.m_from == std::numeric_limits<uint32_t>::max() && cell.m_to == std::numeric_limits<uint32_t>::max());
	return ret;
}

bool ForEachWipcellAddBezier(const Outline *o, const Outline *u, const uint32_t &i, const uint32_t &j, const uint32_t &contourIndex, WIPCell *cells)
{
	Rect bezierBbox;
	Bezier2Bbox(&o->m_points[i], bezierBbox);

	float outlineBboxW = o->m_bbox.m_max.m_x - o->m_bbox.m_min.m_x;
	float outlineBboxH = o->m_bbox.m_max.m_y - o->m_bbox.m_min.m_y;

	auto minX = static_cast<uint32_t>((bezierBbox.m_min.m_x - o->m_bbox.m_min.m_x) / outlineBboxW * o->m_cellCount.m_x);
	auto minY = static_cast<uint32_t>((bezierBbox.m_min.m_y - o->m_bbox.m_min.m_y) / outlineBboxH * o->m_cellCount.m_y);
	auto maxX = static_cast<uint32_t>((bezierBbox.m_max.m_x - o->m_bbox.m_min.m_x) / outlineBboxW * o->m_cellCount.m_x);
	auto maxY = static_cast<uint32_t>((bezierBbox.m_max.m_y - o->m_bbox.m_min.m_y) / outlineBboxH * o->m_cellCount.m_y);

	if (maxX >= o->m_cellCount.m_x)
	{
		maxX = o->m_cellCount.m_x - 1;
	}

	if (maxY >= o->m_cellCount.m_y)
	{
		maxY = o->m_cellCount.m_y - 1;
	}

	bool ret = true;

	for (uint32_t y = minY; y <= maxY; y++)
	{
		for (uint32_t x = minX; x <= maxX; x++)
		{
			WIPCell *cell = &cells[y * o->m_cellCount.m_x + x];

			if (BboxBezier2Intersect(cell->m_bbox, &o->m_points[i]))
			{
				ret &= WipcellAddBezier(o, u, i, j, contourIndex, *cell);
			}
		}
	}

	return ret;
}

bool ForEachWipcellFinishContour(const Outline *o, const Outline *u, const uint32_t &contourIndex, WIPCell *cells, uint32_t &maxStartLength)
{
	bool ret = true;

	for (uint32_t y = 0; y < o->m_cellCount.m_y; y++)
	{
		for (uint32_t x = 0; x < o->m_cellCount.m_x; x++)
		{
			WIPCell *cell = &cells[y * o->m_cellCount.m_x + x];
			ret &= WipcellFinishContour(o, u, contourIndex, *cell, maxStartLength);
		}
	}

	return ret;
}

void CopyWipcellValues(Outline *u, const WIPCell *cells)
{
	u->m_cells = std::vector<uint32_t>(u->m_cellCount.m_x * u->m_cellCount.m_y);

	for (uint32_t y = 0; y < u->m_cellCount.m_y; y++)
	{
		for (uint32_t x = 0; x < u->m_cellCount.m_x; x++)
		{
			uint32_t i = y * u->m_cellCount.m_x + x;
			u->m_cells[i] = cells[i].m_value;
		}
	}
}

void InitWipcells(const Outline *o, WIPCell *cells)
{
	auto size = o->m_bbox.m_max - o->m_bbox.m_min;

	for (uint32_t y = 0; y < o->m_cellCount.m_y; y++)
	{
		for (uint32_t x = 0; x < o->m_cellCount.m_x; x++)
		{
			Rect bbox = { 
				o->m_bbox.m_min + ((Vector2f(x, y) / o->m_cellCount) * size),
				o->m_bbox.m_min + ((Vector2f(x + 1, y + 1) / o->m_cellCount) * size)
			};

			uint32_t i = y * o->m_cellCount.m_x + x;
			cells[i].m_bbox = bbox;
			cells[i].m_from = std::numeric_limits<uint32_t>::max();
			cells[i].m_to = std::numeric_limits<uint32_t>::max();
			cells[i].m_value = 0;
			cells[i].m_startLength = 0;
		}
	}
}

uint32_t OutlineAddFilledLine(Outline *o)
{
	OutlineAddOddPoint(o);

	auto i = static_cast<uint32_t>(o->m_points.size());
	float y = o->m_bbox.m_max.m_y + 1000.0f;
	Vector2f f0 = Vector2f(o->m_bbox.m_min.m_x, y);
	Vector2f f1 = Vector2f(o->m_bbox.m_min.m_x + 10.0f, y);
	Vector2f f2 = Vector2f(o->m_bbox.m_min.m_x + 20.0f, y);
	o->m_points.emplace_back(f0);
	o->m_points.emplace_back(f1);
	o->m_points.emplace_back(f2);

	return i;
}

uint32_t MakeCellFromSingleEdge(const uint32_t &e)
{
	assert(e % 2 == 0);
	return e << 7 | 1;
}

void SetFilledCells(const Outline *u, WIPCell *cells, const uint32_t &filledCell)
{
	for (uint32_t y = 0; y < u->m_cellCount.m_y; y++)
	{
		for (uint32_t x = 0; x < u->m_cellCount.m_x; x++)
		{
			uint32_t i = y * u->m_cellCount.m_x + x;
			WIPCell *cell = &cells[i];

			if (cell->m_value == 0 && IsCellFilled(u, cell->m_bbox))
			{
				cell->m_value = filledCell;
			}
		}
	}
}

bool TryToFitInCellCount(Outline *o)
{
	bool ret = true;

	auto cells = std::vector<WIPCell>(o->m_cellCount.m_x * o->m_cellCount.m_y);
	InitWipcells(o, cells.data());

	Outline u = {};
	u.m_bbox = o->m_bbox;
	u.m_cellCount.m_x = o->m_cellCount.m_x;
	u.m_cellCount.m_y = o->m_cellCount.m_y;

	for (uint32_t contourIndex = 0; contourIndex < o->m_contours.size(); contourIndex++)
	{
		uint32_t contourBegin = o->m_contours[contourIndex].m_begin;
		uint32_t contourEnd = o->m_contours[contourIndex].m_end;

		OutlineAddOddPoint(&u);

		ContourRange urange = { static_cast<uint32_t>(u.m_points.size()), static_cast<uint32_t>(u.m_points.size()) + contourEnd - contourBegin };
		u.m_contours.emplace_back(urange);

		for (uint32_t i = contourBegin; i < contourEnd; i += 2)
		{
			Vector2f &p0 = o->m_points[i];
			Vector2f &p1 = o->m_points[i + 1];
			//Vector2f &p2 = o->points[i + 2];

			auto j = static_cast<uint32_t>(u.m_points.size());
			u.m_points.emplace_back(p0);
			u.m_points.emplace_back(p1);

			ret &= ForEachWipcellAddBezier(o, &u, i, j, contourIndex, cells.data());
		}

		uint32_t max_start_len = 0;
		ret &= ForEachWipcellFinishContour(o, &u, contourIndex, cells.data(), max_start_len);

		uint32_t continuationEnd = contourBegin + max_start_len * 2;

		for (uint32_t i = contourBegin; i < continuationEnd; i += 2)
		{
			u.m_points.emplace_back(o->m_points[i]);
			u.m_points.emplace_back(o->m_points[i + 1]);
		}

		Vector2f &plast = o->m_points[continuationEnd];
		u.m_points.emplace_back(plast);
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
	if (o->m_points.size() > FD_OUTLINE_MAX_POINTS)
	{
		return;
	}

	auto w = o->m_bbox.m_max.m_x - o->m_bbox.m_min.m_x;
	auto h = o->m_bbox.m_max.m_y - o->m_bbox.m_min.m_y;

	float multiplier = 0.5f;

	if (h > w * 1.8f || w > h * 1.8f)
	{
		multiplier = 1.0f;
	}

	auto c = Uint32ToPow2(static_cast<uint32_t>(std::sqrt(o->m_points.size() * 0.75f)));

	o->m_cellCount.m_x = c;
	o->m_cellCount.m_y = c;

	if (h > w * 1.8f)
	{
		o->m_cellCount.m_x /= 2;
	}

	if (w > h * 1.8f)
	{
		o->m_cellCount.m_y /= 2;
	}

	while (true)
	{
		if (TryToFitInCellCount(o))
		{
			break;
		}

		if (o->m_cellCount.m_x > 64 || o->m_cellCount.m_y > 64)
		{
			o->m_cellCount.m_x = 0;
			o->m_cellCount.m_y = 0;
			return;
		}

		if (o->m_cellCount.m_x == o->m_cellCount.m_y)
		{
			if (w > h)
			{
				o->m_cellCount.m_x *= 2;
			}
			else
			{
				o->m_cellCount.m_y *= 2;
			}
		}
		else
		{
			if (o->m_cellCount.m_x < o->m_cellCount.m_y)
			{
				o->m_cellCount.m_x *= 2;
			}
			else
			{
				o->m_cellCount.m_y *= 2;
			}
		}
	}
}

void OutlineSubdivide(Outline *o)
{
	Outline u = {};
	u.m_bbox = o->m_bbox;

	for (uint32_t contourIndex = 0; contourIndex < o->m_contours.size(); contourIndex++)
	{
		uint32_t contourBegin = o->m_contours[contourIndex].m_begin;
		uint32_t contour_end = o->m_contours[contourIndex].m_end;

		OutlineAddOddPoint(&u);

		ContourRange urange = { static_cast<uint32_t>(u.m_points.size()), std::numeric_limits<uint32_t>::max() };
		u.m_contours.emplace_back(urange);

		for (uint32_t i = contourBegin; i < contour_end; i += 2)
		{
			Vector2f &p0 = o->m_points[i];
			//Vector2f &p1 = o->points[i + 1];
			//Vector2f &p2 = o->points[i + 2];

			Vector2f newp[3];
			Bezier2Split_3P(newp, &o->m_points[i], 0.5f);

			u.m_points.emplace_back(p0);
			u.m_points.emplace_back(newp[0]);
			u.m_points.emplace_back(newp[1]);
			u.m_points.emplace_back(newp[2]);
		}

		u.m_contours[contourIndex].m_end = static_cast<uint32_t>(u.m_points.size());
		u.m_points.emplace_back(o->m_points[contour_end]);
	}

	*o = u;
}

void OutlineFixCorners(Outline *o)
{
	float fixDist = 0.001f;

	for (uint32_t contourIndex = 0; contourIndex < o->m_contours.size(); contourIndex++)
	{
		uint32_t contourBegin = o->m_contours[contourIndex].m_begin;
		uint32_t contourEnd = o->m_contours[contourIndex].m_end;

		for (uint32_t i = contourBegin; i < contourEnd; i += 2)
		{
			uint32_t prev = i - 1;

			if (contourBegin == i)
			{
				prev = contourEnd - 1;
			}

			Vector2f &r = o->m_points[prev];
			Vector2f &p0 = o->m_points[i];
			Vector2f &p1 = o->m_points[i + 1];

			Vector2f v0 = r - p0;
			Vector2f v1 = p1 - p0;

			v0 = v0.Normalize();
			v1 = v1.Normalize();

			float angle = std::acos(v0.Dot(v1));

			if (angle <= Maths::Pi / 2.0f * 1.025f)
			{
				v0 *= fixDist;
				v1 *= fixDist;

				Vector2f f1 = p0 - v0;
				Vector2f f0 = p0 - v1;

				OutlineAddOddPoint(o);

				if (o->m_cornerFixBegin == 0)
				{
					o->m_cornerFixBegin = static_cast<uint32_t>(o->m_points.size());
				}

				o->m_points.emplace_back(f0);
				o->m_points.emplace_back(f1);
			}
		}
	}
}

void OutlineFixThinLines(Outline *o)
{
	// TODO: Optimize
	Outline u = {};
	u.m_bbox = o->m_bbox;

	for (uint32_t contourIndex = 0; contourIndex < o->m_contours.size(); contourIndex++)
	{
		uint32_t contourBegin = o->m_contours[contourIndex].m_begin;
		uint32_t contourEnd = o->m_contours[contourIndex].m_end;

		OutlineAddOddPoint(&u);

		ContourRange urange = { static_cast<uint32_t>(u.m_points.size()), std::numeric_limits<uint32_t>::max() };
		u.m_contours.emplace_back(urange);

		for (uint32_t i = contourBegin; i < contourEnd; i += 2)
		{
			Vector2f &p0 = o->m_points[i];
			Vector2f &p1 = o->m_points[i + 1];
			Vector2f &p2 = o->m_points[i + 2];

			Vector2f mid = p0.Lerp(p2, 0.5f);
			Vector2f midp1 = p1 - mid;

			Vector2f bezier[] = { p0, p0, p2 };

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
					Vector2f &q0 = o->points[j];
					Vector2f &q2 = o->points[j + 2];

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
				Vector2f &q0 = o->points[j];
				Vector2f &q2 = o->points[j + 2];

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

				Vector2f &q0 = o->m_points[j];
				//Vector2f &q1 = o->points[j + 1];
				Vector2f &q2 = o->m_points[j + 2];

				if (Bezier2LineIsIntersecting(bezier, q0, q2))
				{
					subdivide = true;
				}
			}

			if (subdivide)
			{
				Vector2f newp[3];
				Bezier2Split_3P(newp, &o->m_points[i], 0.5f);

				u.m_points.emplace_back(p0);
				u.m_points.emplace_back(newp[0]);
				u.m_points.emplace_back(newp[1]);
				u.m_points.emplace_back(newp[2]);
			}
			else
			{
				u.m_points.emplace_back(p0);
				u.m_points.emplace_back(p1);
			}
		}

		u.m_contours[contourIndex].m_end = static_cast<uint32_t>(u.m_points.size());
		u.m_points.emplace_back(o->m_points[contourEnd]);
	}

	*o = u;
}

void OutlineCbox(Outline *o, Rect *cbox)
{
	if (o->m_points.empty())
	{
		return;
	}

	cbox->m_min.m_x = o->m_points[0].m_x;
	cbox->m_min.m_y = o->m_points[0].m_y;
	cbox->m_max.m_x = o->m_points[0].m_x;
	cbox->m_max.m_y = o->m_points[0].m_y;

	for (const auto &point : o->m_points)
	{
		cbox->m_min.m_x = std::min(cbox->m_min.m_x, point.m_x);
		cbox->m_min.m_y = std::min(cbox->m_min.m_y, point.m_y);
		cbox->m_max.m_x = std::max(cbox->m_max.m_x, point.m_x);
		cbox->m_max.m_y = std::max(cbox->m_max.m_y, point.m_y);
	}
}

uint16_t GenU16Value(const float &x, const float &min, const float &max)
{
	return static_cast<uint16_t>((x - min) / (max - min) * std::numeric_limits<uint16_t>::max());
}

void OutlineU16Points(Outline *o, Rect *cbox, Vector2us *pout)
{
	OutlineCbox(o, cbox);

	for (uint32_t i = 0; i < o->m_points.size(); i++)
	{
		float x = o->m_points[i].m_x;
		float y = o->m_points[i].m_y;

		pout[i].m_x = GenU16Value(x, cbox->m_min.m_x, cbox->m_max.m_x);
		pout[i].m_y = GenU16Value(y, cbox->m_min.m_y, cbox->m_max.m_y);
	}
}
}
