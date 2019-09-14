#pragma once

#include "Geometry.hpp"

struct FT_Vector_;
typedef FT_Vector_ FT_Vector;
struct FT_Outline_;
typedef FT_Outline_ FT_Outline;

namespace acid
{
class ACID_EXPORT WipCell
{
public:
	Rect m_bbox;
	uint32_t m_value = 0;
	uint32_t m_from = 0;
	uint32_t m_to = 0;
	uint32_t m_startLength = 0;
};

class ACID_EXPORT ContourRange
{
public:
	uint32_t m_begin = 0, m_end = 0;
};

class ACID_EXPORT Outline
{
public:
	Rect m_bbox;

	std::vector<Vector2f> m_points;

	std::vector<ContourRange> m_contours;

	std::vector<uint32_t> m_cells;
	Vector2ui m_cellCount;

	uint32_t m_cornerFixBegin = 0;
};

static void OutlineAddOddPoint(Outline *o);

static void ConvertPoint(const FT_Vector *v, Vector2f &out);

static int32_t MoveToFunc(const FT_Vector *to, Outline *o);

static int32_t LineToFunc(const FT_Vector *to, Outline *o);

static int32_t ConicToFunc(const FT_Vector *control, const FT_Vector *to, Outline *o);

static int32_t CubicToFunc(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o);

void OutlineConvert(FT_Outline *outline, Outline *o);

void OutlineDecompose(FT_Outline *outline, Outline *o);

static uint32_t CellAddRange(uint32_t cell, uint32_t from, uint32_t to);

static bool IsCellFilled(const Outline *o, const Rect &bbox);

static bool WipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell &cell);

static bool WipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell &cell, uint32_t &maxStartLength);

static bool ForEachWipcellAddBezier(const Outline *o, const Outline *u, uint32_t i, uint32_t j, uint32_t contourIndex, WipCell *cells);

static bool ForEachWipcellFinishContour(const Outline *o, const Outline *u, uint32_t contourIndex, WipCell *cells, uint32_t &maxStartLength);

static void CopyWipcellValues(Outline *u, const WipCell *cells);

static void InitWipcells(const Outline *o, WipCell *cells);

static uint32_t OutlineAddFilledLine(Outline *o);

static uint32_t MakeCellFromSingleEdge(uint32_t e);

static void SetFilledCells(const Outline *u, WipCell *cells, uint32_t filledCell);

static bool TryToFitInCellCount(Outline *o);

static uint32_t Uint32ToPow2(uint32_t v);

void OutlineMakeCells(Outline *o);

void OutlineSubdivide(Outline *o);

void OutlineFixCorners(Outline *o);

static void OutlineFixThinLines(Outline *o);

void OutlineCbox(Outline *o, Rect *cbox);

static uint16_t GenU16Value(float x, float min, float max);

void OutlineU16Points(Outline *o, Rect *cbox, Vector2us *pout);
}
