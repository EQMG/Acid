/*#pragma once

#include <vector>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftbbox.h>
#include <freetype/ftoutln.h>
#include <Maths/Vector2.hpp>
#include "Geometry.hpp"

namespace acid
{
#define FT_CHECK(r) do { FT_Error err = (r); assert(!err); } while (0)

#define OUTLINE_MAX_POINTS (255 * 2)

	struct ContourRange
	{
		uint32_t begin, end;
	};

	class Cell;

	class Outline
	{
	public:
		Rect m_bbox;

		Vector2 *m_points;
		uint32_t m_pointCount;
		uint32_t m_pointCapacity;

		ContourRange *m_contours;
		uint32_t m_contourCount;
		uint32_t m_contourCapacity;

		uint32_t *m_cells;
		uint32_t m_cellCountX;
		uint32_t m_cellCountY;

		void AddPoint(const Vector2 &point);

		void AddContour(const ContourRange &range);

		void AddOddPoint();

		void Decompose(FT_Outline *outline);

		bool IsCellFilled(const Rect &bbox) const;

		void CopyCellValues(Cell *wipCells);

		void InitCells(Cell *wipCells);



		uint32_t AddFilledLine();

		bool TryFitCellCount();

		void MakeCells();

		void Subdivide();

		void FixThinLines();

		void Convert(FT_Outline *outline);

		void Destroy();

		Rect GetCbox() const;
	};

	template<typename T>
	static inline void dyn_array_grow(T **data, uint32_t *capacity, size_t element_size)
	{
		*capacity = *capacity ? *capacity * 2 : 8;
		T *new_data = (T *) realloc(*data, *capacity * element_size);
		*data = new_data;
	}

	static Vector2 convert_point(const FT_Vector *v);


	static int move_to_func(const FT_Vector *to, Outline &o);

	static int line_to_func(const FT_Vector *to, Outline &o);

	static int conic_to_func(const FT_Vector *control, const FT_Vector *to, Outline &o);

	static int cubic_to_func(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline &o);




	static uint32_t make_cell_from_single_edge(const uint32_t &e);

	static uint32_t uint32_to_pow2(const uint32_t &v);

	static uint16_t gen_u16_value(const float &x, const float &min, const float &max);
}*/
