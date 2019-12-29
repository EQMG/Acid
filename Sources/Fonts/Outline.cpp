#include "Outline.hpp"

#include <ft2build.h>
#include FT_BBOX_H
#include FT_OUTLINE_H

#include "Maths/Maths.hpp"
#include "Helpers/Enumerate.hpp"

namespace acid {
static const uint32_t FD_OUTLINE_MAX_POINTS = 255 * 2;


void Outline::OutlineConvert(FT_Outline *outline) {
	Decompose(outline);
	//FixCorners();
	//Subdivide();
	FixThinLines();
	MakeCells();
}

void Outline::Decompose(FT_Outline *outline) {
	*this = {};

	FT_BBox outlineBbox;

	if (FT_Outline_Get_BBox(outline, &outlineBbox) != 0) {
		throw std::runtime_error("Freetype failed to get a bbox outline");
	}

	m_bbox.m_min.m_x = static_cast<float>(outlineBbox.xMin) / 64.0f;
	m_bbox.m_min.m_y = static_cast<float>(outlineBbox.yMin) / 64.0f;
	m_bbox.m_max.m_x = static_cast<float>(outlineBbox.xMax) / 64.0f;
	m_bbox.m_max.m_y = static_cast<float>(outlineBbox.yMax) / 64.0f;

	FT_Outline_Funcs funcs = {};
	funcs.move_to = reinterpret_cast<FT_Outline_MoveToFunc>(MoveToFunc);
	funcs.line_to = reinterpret_cast<FT_Outline_LineToFunc>(LineToFunc);
	funcs.conic_to = reinterpret_cast<FT_Outline_ConicToFunc>(ConicToFunc);
	funcs.cubic_to = reinterpret_cast<FT_Outline_CubicToFunc>(CubicToFunc);

	if (FT_Outline_Decompose(outline, &funcs, this) != 0) {
		throw std::runtime_error("Freetype failed to decompose a outline");
	}

	if (!m_contours.empty()) {
		m_contours[m_contours.size() - 1].m_end = static_cast<uint32_t>(m_points.size() - 1);
	}
}

void Outline::FixCorners() {
	auto fixDist = 0.001f;

	for (auto &&[contourIndex, contour] : Enumerate(m_contours)) {
		for (auto i = contour.m_begin; i < contour.m_end; i += 2) {
			auto prev = i - 1;
			if (contour.m_begin == i)
				prev = contour.m_end - 1;

			auto &r = m_points[prev];
			auto &p0 = m_points[i];
			auto &p1 = m_points[i + 1];

			auto v0 = r - p0;
			auto v1 = p1 - p0;

			v0 = v0.Normalize();
			v1 = v1.Normalize();

			float angle = std::acos(v0.Dot(v1));

			if (angle <= Maths::Pi<float> / 2.0f * 1.025f) {
				v0 *= fixDist;
				v1 *= fixDist;

				auto f1 = p0 - v0;
				auto f0 = p0 - v1;

				AddOddPoint();

				if (m_cornerFixBegin == 0) {
					m_cornerFixBegin = static_cast<uint32_t>(m_points.size());
				}

				m_points.emplace_back(f0);
				m_points.emplace_back(f1);
			}
		}
	}
}

void Outline::Subdivide() {
	Outline u = {};
	u.m_bbox = m_bbox;

	for (auto &&[contourIndex, contour] : Enumerate(m_contours)) {
		u.AddOddPoint();

		ContourRange urange = {
			static_cast<uint32_t>(u.m_points.size()),
			std::numeric_limits<uint32_t>::max()
		};
		u.m_contours.emplace_back(urange);

		for (uint32_t i = contour.m_begin; i < contour.m_end; i += 2) {
			auto &p0 = m_points[i];
			//auto &p1 = points[i + 1];
			//auto &p2 = points[i + 2];

			Vector2f newp[3];
			Rect::Bezier2Split3P(newp, &m_points[i], 0.5f);

			u.m_points.emplace_back(p0);
			u.m_points.emplace_back(newp[0]);
			u.m_points.emplace_back(newp[1]);
			u.m_points.emplace_back(newp[2]);
		}

		u.m_contours[contourIndex].m_end = static_cast<uint32_t>(u.m_points.size());
		u.m_points.emplace_back(m_points[contour.m_end]);
	}

	*this = u;
}

void Outline::FixThinLines() {
	// TODO: Optimize
	Outline u = {};
	u.m_bbox = m_bbox;

	for (auto &&[contourIndex, contour] : Enumerate(m_contours)) {
		u.AddOddPoint();

		ContourRange urange = {
			static_cast<uint32_t>(u.m_points.size()),
			std::numeric_limits<uint32_t>::max()
		};
		u.m_contours.emplace_back(urange);

		for (auto i = contour.m_begin; i < contour.m_end; i += 2) {
			auto &p0 = m_points[i];
			auto &p1 = m_points[i + 1];
			auto &p2 = m_points[i + 2];

			auto mid = p0.Lerp(p2, 0.5f);
			auto midp1 = p1 - mid;

			Vector2f bezier[] = {p0, p0, p2};

			bezier[1] += midp1;
			/*bool subdivide = false;

			if (i > 2) {
				auto jbegin = contourBegin;

				if (i == contourEnd - 2) {
					jbegin += 2;
				}

				for (auto j = jbegin; j < i - 2; j += 2) {
					auto &q0 = points[j];
					auto &q2 = points[j + 2];

					if (Bezier2LineIsIntersecting(bezier, q0, q2)) {
						subdivide = true;
					}
				}
			}

			uint32_t jend = contourEnd;

			if (i == contourBegin) {
				jend -= 2;
			}

			for (auto j = i + 2; j < jend; j += 2) {
				auto &q0 = points[j];
				auto &q2 = points[j + 2];

				if (Bezier2LineIsIntersecting(bezier, q0, q2)) {
					subdivide = true;
				}
			}*/

			bool subdivide = false;

			for (auto j = contour.m_begin; j < contour.m_end; j += 2) {
				if (i == contour.m_begin && j == contour.m_end - 2) {
					continue;
				}

				if (i == contour.m_end - 2 && j == contour.m_begin) {
					continue;
				}

				if (j + 2 >= i && j <= i + 2) {
					continue;
				}

				auto &q0 = m_points[j];
				//auto &q1 = points[j + 1];
				auto &q2 = m_points[j + 2];

				if (Rect::Bezier2LineIsIntersecting(bezier, q0, q2)) {
					subdivide = true;
				}
			}

			if (subdivide) {
				Vector2f newp[3];
				Rect::Bezier2Split3P(newp, &m_points[i], 0.5f);

				u.m_points.emplace_back(p0);
				u.m_points.emplace_back(newp[0]);
				u.m_points.emplace_back(newp[1]);
				u.m_points.emplace_back(newp[2]);
			} else {
				u.m_points.emplace_back(p0);
				u.m_points.emplace_back(p1);
			}
		}

		u.m_contours[contourIndex].m_end = static_cast<uint32_t>(u.m_points.size());
		u.m_points.emplace_back(m_points[contour.m_end]);
	}

	*this = u;
}

void Outline::MakeCells() {
	if (m_points.size() > FD_OUTLINE_MAX_POINTS) {
		return;
	}

	auto w = m_bbox.m_max.m_x - m_bbox.m_min.m_x;
	auto h = m_bbox.m_max.m_y - m_bbox.m_min.m_y;

	auto multiplier = 0.5f;

	if (h > w * 1.8f || w > h * 1.8f) {
		multiplier = 1.0f;
	}

	auto c = Uint32ToPow2(static_cast<uint32_t>(std::sqrt(m_points.size() * 0.75f)));

	m_cellCount.m_x = c;
	m_cellCount.m_y = c;

	if (h > w * 1.8f) {
		m_cellCount.m_x /= 2;
	}

	if (w > h * 1.8f) {
		m_cellCount.m_y /= 2;
	}

	while (true) {
		if (TryToFitInCellCount()) {
			break;
		}

		if (m_cellCount.m_x > 64 || m_cellCount.m_y > 64) {
			m_cellCount.m_x = 0;
			m_cellCount.m_y = 0;
			return;
		}

		if (m_cellCount.m_x == m_cellCount.m_y) {
			if (w > h) {
				m_cellCount.m_x *= 2;
			} else {
				m_cellCount.m_y *= 2;
			}
		} else {
			if (m_cellCount.m_x < m_cellCount.m_y) {
				m_cellCount.m_x *= 2;
			} else {
				m_cellCount.m_y *= 2;
			}
		}
	}
}

int32_t Outline::MoveToFunc(const FT_Vector *to, Outline *o) {
	Vector2f p;

	if (!o->m_contours.empty()) {
		o->m_contours[o->m_contours.size() - 1].m_end = static_cast<uint32_t>(o->m_points.size() - 1);
		o->m_points.emplace_back(p);
	}

	assert(o->m_points.size() % 2 == 0);

	ContourRange range = {
		static_cast<uint32_t>(o->m_points.size()),
		std::numeric_limits<uint32_t>::max()
	};
	o->m_contours.emplace_back(range);

	p = ConvertPoint(to);
	o->m_points.emplace_back(p);
	return 0;
}

int32_t Outline::LineToFunc(const FT_Vector *to, Outline *o) {
	auto last = static_cast<uint32_t>(o->m_points.size() - 1);

	Vector2f toP = ConvertPoint(to);
	auto p = o->m_points[last].Lerp(toP, 0.5f);
	o->m_points.emplace_back(p);
	o->m_points.emplace_back(toP);
	return 0;
}

int32_t Outline::ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o) {
	o->m_points.emplace_back(ConvertPoint(control));
	o->m_points.emplace_back(ConvertPoint(to));
	return 0;
}

int32_t Outline::CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o) {
	return LineToFunc(to, o);
}

void Outline::AddOddPoint() {
	if (m_points.size() % 2 != 0) {
		Vector2f p(m_bbox.m_min.m_x, m_bbox.m_min.m_y);
		m_points.emplace_back(p);
	}
}

uint32_t Outline::AddFilledLine() {
	AddOddPoint();

	auto i = static_cast<uint32_t>(m_points.size());
	auto y = m_bbox.m_max.m_y + 1000.0f;
	Vector2f f0(m_bbox.m_min.m_x, y);
	Vector2f f1(m_bbox.m_min.m_x + 10.0f, y);
	Vector2f f2(m_bbox.m_min.m_x + 20.0f, y);
	m_points.emplace_back(f0);
	m_points.emplace_back(f1);
	m_points.emplace_back(f2);

	return i;
}

Rect Outline::GetCbox() const {
	if (m_points.empty()) {
		return {};
	}
	
	Rect cbox;
	cbox.m_min.m_x = m_points[0].m_x;
	cbox.m_min.m_y = m_points[0].m_y;
	cbox.m_max.m_x = m_points[0].m_x;
	cbox.m_max.m_y = m_points[0].m_y;

	for (const auto &point : m_points) {
		cbox.m_min.m_x = std::min(cbox.m_min.m_x, point.m_x);
		cbox.m_min.m_y = std::min(cbox.m_min.m_y, point.m_y);
		cbox.m_max.m_x = std::max(cbox.m_max.m_x, point.m_x);
		cbox.m_max.m_y = std::max(cbox.m_max.m_y, point.m_y);
	}

	return cbox;
}

Rect Outline::GetU16Points(Vector2us *pout) const {
	auto cbox = GetCbox();

	for (auto &&[i, point] : Enumerate(m_points)) {
		pout[i].m_x = GenU16Value(point.m_x, cbox.m_min.m_x, cbox.m_max.m_x);
		pout[i].m_y = GenU16Value(point.m_y, cbox.m_min.m_y, cbox.m_max.m_y);
	}

	return cbox;
}

bool Outline::IsCellFilled(const Rect &bbox) {
	// TODO: Optimize
	Vector2f p((bbox.m_max.m_x + bbox.m_min.m_x) / 2.0f, (bbox.m_max.m_y + bbox.m_min.m_y) / 2.0f);

	auto minDist = std::numeric_limits<float>::max();
	auto v = std::numeric_limits<float>::max();
	auto j = std::numeric_limits<uint32_t>::max();

	for (auto &contour : m_contours) {
		auto contourBegin = contour.m_begin;
		auto contourEnd = contour.m_end;

		for (uint32_t i = contourBegin; i < contourEnd; i += 2) {
			auto p0 = m_points[i];
			auto p1 = m_points[i + 1];
			auto p2 = m_points[i + 2];

			auto t = Rect::LineCalculateT(p0, p2, p);

			auto p02 = p0.Lerp(p2, t);

			auto uDist = p02.Distance(p);

			if (uDist < minDist + 0.0001f) {
				auto d = Rect::LineSignedDistance(p0, p2, p);

				if (uDist >= minDist && i > contourBegin) {
					auto lastD = i == contourEnd - 2 && j == contourBegin ? Rect::LineSignedDistance(p0, p2, m_points[contourBegin + 2]) :
						Rect::LineSignedDistance(p0, p2, m_points[i - 2]);

					if (lastD < 0.0f) {
						v = std::max(d, v);
					} else {
						v = std::min(d, v);
					}
				} else {
					v = d;
				}

				minDist = std::min(minDist, uDist);
				j = i;
			}
		}
	}

	return v > 0.0f;
}

Vector2f Outline::ConvertPoint(const FT_Vector *v) {
	Vector2f out;
	out.m_x = static_cast<float>(v->x) / 64.0f;
	out.m_y = static_cast<float>(v->y) / 64.0f;
	return out;
}

uint32_t Outline::CellAddRange(uint32_t cell, uint32_t from, uint32_t to) {
	assert(from % 2 == 0 && to % 2 == 0);

	from /= 2;
	to /= 2;

	if (from >= std::numeric_limits<uint8_t>::max()) {
		return 0;
	}

	if (to >= std::numeric_limits<uint8_t>::max()) {
		return 0;
	}

	auto length = to - from;

	if (length <= 3 && (cell & 0x03) == 0) {
		cell |= from << 8;
		cell |= length;
		return cell;
	}

	if (length > 7) {
		return 0;
	}

	if ((cell & 0x1C) == 0) {
		cell |= from << 16;
		cell |= length << 2;
		return cell;
	}

	if ((cell & 0xE0) == 0) {
		cell |= from << 24;
		cell |= length << 5;
		return cell;
	}

	return 0;
}

uint32_t Outline::MakeCellFromSingleEdge(uint32_t e) {
	assert(e % 2 == 0);
	return e << 7 | 1;
}

uint32_t Outline::Uint32ToPow2(uint32_t v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

uint16_t Outline::GenU16Value(float x, float min, float max) {
	return static_cast<uint16_t>((x - min) / (max - min) * std::numeric_limits<uint16_t>::max());
}

bool Outline::WipCell::WipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell &cell) {
	auto result = true;
	auto ucontourBegin = u->m_contours[contourIndex].m_begin;

	if (cell.m_to != std::numeric_limits<uint32_t>::max() && cell.m_to != j) {
		assert(cell.m_to < j);

		if (cell.m_from == ucontourBegin) {
			assert(cell.m_to % 2 == 0);
			assert(cell.m_from % 2 == 0);

			cell.m_startLength = (cell.m_to - cell.m_from) / 2;
		} else {
			cell.m_value = CellAddRange(cell.m_value, cell.m_from, cell.m_to);

			if (!cell.m_value) {
				result = false;
			}
		}

		cell.m_from = j;
	} else {
		if (cell.m_from == std::numeric_limits<uint32_t>::max()) {
			cell.m_from = j;
		}
	}

	cell.m_to = j + 2;
	return result;
}

bool Outline::WipCell::WipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell &cell, uint32_t &maxStartLength) {
	bool result = true;
	auto ucontourBegin = u->m_contours[contourIndex].m_begin;
	auto ucontourEnd = u->m_contours[contourIndex].m_end;

	if (cell.m_to < ucontourEnd) {
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, cell.m_to);

		if (!cell.m_value) {
			result = false;
		}

		cell.m_from = std::numeric_limits<uint32_t>::max();
		cell.m_to = std::numeric_limits<uint32_t>::max();
	}

	assert(cell.m_to == std::numeric_limits<uint32_t>::max() || cell.m_to == ucontourEnd);
	cell.m_to = std::numeric_limits<uint32_t>::max();

	if (cell.m_from != std::numeric_limits<uint32_t>::max() && cell.m_startLength != 0) {
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, ucontourEnd + cell.m_startLength * 2);

		if (!cell.m_value) {
			result = false;
		}

		maxStartLength = std::max(maxStartLength, cell.m_startLength);
		cell.m_from = std::numeric_limits<uint32_t>::max();
		cell.m_startLength = 0;
	}

	if (cell.m_from != std::numeric_limits<uint32_t>::max()) {
		cell.m_value = CellAddRange(cell.m_value, cell.m_from, ucontourEnd);

		if (!cell.m_value) {
			result = false;
		}

		cell.m_from = std::numeric_limits<uint32_t>::max();
	}

	if (cell.m_startLength != 0) {
		cell.m_value = CellAddRange(cell.m_value, ucontourBegin, ucontourBegin + cell.m_startLength * 2);

		if (!cell.m_value) {
			result = false;
		}

		cell.m_startLength = 0;
	}

	assert(cell.m_from == std::numeric_limits<uint32_t>::max() && cell.m_to == std::numeric_limits<uint32_t>::max());
	return result;
}

bool Outline::WipCell::ForEachWipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell *cells) {
	Rect bezierBbox(&o->m_points[i]);

	auto outlineBboxW = o->m_bbox.m_max.m_x - o->m_bbox.m_min.m_x;
	auto outlineBboxH = o->m_bbox.m_max.m_y - o->m_bbox.m_min.m_y;

	auto minX = static_cast<uint32_t>((bezierBbox.m_min.m_x - o->m_bbox.m_min.m_x) / outlineBboxW * o->m_cellCount.m_x);
	auto minY = static_cast<uint32_t>((bezierBbox.m_min.m_y - o->m_bbox.m_min.m_y) / outlineBboxH * o->m_cellCount.m_y);
	auto maxX = static_cast<uint32_t>((bezierBbox.m_max.m_x - o->m_bbox.m_min.m_x) / outlineBboxW * o->m_cellCount.m_x);
	auto maxY = static_cast<uint32_t>((bezierBbox.m_max.m_y - o->m_bbox.m_min.m_y) / outlineBboxH * o->m_cellCount.m_y);

	if (maxX >= o->m_cellCount.m_x) {
		maxX = o->m_cellCount.m_x - 1;
	}

	if (maxY >= o->m_cellCount.m_y) {
		maxY = o->m_cellCount.m_y - 1;
	}

	bool result = true;

	for (uint32_t y = minY; y <= maxY; y++) {
		for (uint32_t x = minX; x <= maxX; x++) {
			auto cell = &cells[y * o->m_cellCount.m_x + x];

			if (cell->m_bbox.BboxBezier2Intersect(&o->m_points[i])) {
				result &= WipcellAddBezier(o, u, i, j, contourIndex, *cell);
			}
		}
	}

	return result;
}

bool Outline::WipCell::ForEachWipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell *cells, uint32_t &maxStartLength) {
	bool ret = true;

	for (uint32_t y = 0; y < o->m_cellCount.m_y; y++) {
		for (uint32_t x = 0; x < o->m_cellCount.m_x; x++) {
			auto cell = &cells[y * o->m_cellCount.m_x + x];
			ret &= WipcellFinishContour(o, u, contourIndex, *cell, maxStartLength);
		}
	}

	return ret;
}

bool Outline::TryToFitInCellCount() {
	bool result = true;

	std::vector<WipCell> cells(m_cellCount.m_x * m_cellCount.m_y);
	InitWipcells(cells);

	Outline u = {};
	u.m_bbox = m_bbox;
	u.m_cellCount.m_x = m_cellCount.m_x;
	u.m_cellCount.m_y = m_cellCount.m_y;

	for (auto &&[contourIndex, contour] : Enumerate(m_contours)) {
		u.AddOddPoint();

		ContourRange urange = {
			static_cast<uint32_t>(u.m_points.size()),
			static_cast<uint32_t>(u.m_points.size()) + contour.m_end - contour.m_begin
		};
		u.m_contours.emplace_back(urange);

		for (uint32_t i = contour.m_begin; i < contour.m_end; i += 2) {
			auto &p0 = m_points[i];
			auto &p1 = m_points[i + 1];
			//auto &p2 = points[i + 2];

			auto j = static_cast<uint32_t>(u.m_points.size());
			u.m_points.emplace_back(p0);
			u.m_points.emplace_back(p1);

			result &= WipCell::ForEachWipcellAddBezier(this, &u, i, j, contourIndex, cells.data());
		}

		uint32_t maxStartLen = 0;
		result &= WipCell::ForEachWipcellFinishContour(this, &u, contourIndex, cells.data(), maxStartLen);

		auto continuationEnd = contour.m_begin + maxStartLen * 2;

		for (uint32_t i = contour.m_begin; i < continuationEnd; i += 2) {
			u.m_points.emplace_back(m_points[i]);
			u.m_points.emplace_back(m_points[i + 1]);
		}

		auto &plast = m_points[continuationEnd];
		u.m_points.emplace_back(plast);
	}

	if (!result) {
		return result;
	}

	auto filledLine = u.AddFilledLine();
	auto filledCell = MakeCellFromSingleEdge(filledLine);
	u.SetFilledCells(cells, filledCell);

	u.CopyWipcellValues(cells);

	*this = u;
	return result;
}

void Outline::CopyWipcellValues(const std::vector<WipCell> &cells) {
	m_cells = std::vector<uint32_t>(m_cellCount.m_x * m_cellCount.m_y);

	for (uint32_t y = 0; y < m_cellCount.m_y; y++) {
		for (uint32_t x = 0; x < m_cellCount.m_x; x++) {
			auto i = y * m_cellCount.m_x + x;
			m_cells[i] = cells[i].m_value;
		}
	}
}

void Outline::InitWipcells(std::vector<WipCell> &cells) const {
	auto size = m_bbox.m_max - m_bbox.m_min;

	for (uint32_t y = 0; y < m_cellCount.m_y; y++) {
		for (uint32_t x = 0; x < m_cellCount.m_x; x++) {
			Rect bbox = {
				m_bbox.m_min + ((Vector2f(x, y) / m_cellCount) * size),
				m_bbox.m_min + ((Vector2f(x + 1, y + 1) / m_cellCount) * size)
			};

			auto i = y * m_cellCount.m_x + x;
			cells[i].m_bbox = bbox;
			cells[i].m_from = std::numeric_limits<uint32_t>::max();
			cells[i].m_to = std::numeric_limits<uint32_t>::max();
			cells[i].m_value = 0;
			cells[i].m_startLength = 0;
		}
	}
}

void Outline::SetFilledCells(std::vector<WipCell> &cells, uint32_t filledCell) {
	for (uint32_t y = 0; y < m_cellCount.m_y; y++) {
		for (uint32_t x = 0; x < m_cellCount.m_x; x++) {
			auto i = y * m_cellCount.m_x + x;
			auto cell = &cells[i];

			if (cell->m_value == 0 && IsCellFilled(cell->m_bbox)) {
				cell->m_value = filledCell;
			}
		}
	}
}
}
