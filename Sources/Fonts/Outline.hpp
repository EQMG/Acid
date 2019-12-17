#pragma once

#include "Geometry.hpp"

struct FT_Vector_;
typedef FT_Vector_ FT_Vector;
struct FT_Outline_;
typedef FT_Outline_ FT_Outline;

namespace acid {
class ACID_EXPORT Outline {
public:
	void OutlineConvert(FT_Outline *outline);

	void Decompose(FT_Outline *outline);
	void FixCorners();
	void Subdivide();
	void FixThinLines();
	void MakeCells();

private:
	static int32_t MoveToFunc(const FT_Vector *to, Outline *o);
	static int32_t LineToFunc(const FT_Vector *to, Outline *o);
	static int32_t ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o);
	static int32_t CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o);
	
	void AddOddPoint();
	
	uint32_t AddFilledLine();
	Rect GetCbox() const;
	Rect GetU16Points(Vector2us *pout) const;
	bool IsCellFilled(const Rect &bbox);

	static Vector2f ConvertPoint(const FT_Vector *v);
	static uint32_t CellAddRange(uint32_t cell, uint32_t from, uint32_t to);
	static uint32_t MakeCellFromSingleEdge(uint32_t e);
	static uint32_t Uint32ToPow2(uint32_t v);
	static uint16_t GenU16Value(float x, float min, float max);
	
	class WipCell {
	public:
		static bool WipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell &cell);
		static bool WipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell &cell, uint32_t &maxStartLength);
		static bool ForEachWipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell *cells);
		static bool ForEachWipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell *cells, uint32_t &maxStartLength);

		Rect m_bbox;
		uint32_t m_value = 0;
		uint32_t m_from = 0;
		uint32_t m_to = 0;
		uint32_t m_startLength = 0;
	};

	bool TryToFitInCellCount();
	void CopyWipcellValues(const std::vector<WipCell> &cells);
	void InitWipcells(std::vector<WipCell> &cells) const;
	void SetFilledCells(std::vector<WipCell> &cells, uint32_t filledCell);

public:
	class ContourRange {
	public:
		uint32_t m_begin = 0, m_end = 0;
	};

	Rect m_bbox;
	std::vector<Vector2f> m_points;
	std::vector<ContourRange> m_contours;
	std::vector<uint32_t> m_cells;
	Vector2ui m_cellCount;
	uint32_t m_cornerFixBegin = 0;
};

}
