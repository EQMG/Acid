#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BBOX_H
#include FT_OUTLINE_H
#include "Geometry.hpp"

namespace acid
{
#define FT_CHECK(r) do { FT_Error err = (r); assert(!err); } while (0)

#define FD_OUTLINE_MAX_POINTS (255 * 2)

	struct ContourRange
	{
		uint32_t begin, end;
	};

	struct Outline
	{
		Rect bbox;

		Vector2 *points;
		uint32_t num_of_points;
		uint32_t point_capacity;

		ContourRange *contours;
		uint32_t num_of_contours;
		uint32_t contour_capacity;

		uint32_t *cells;
		uint32_t cell_count_x;
		uint32_t cell_count_y;

		uint32_t corner_fix_begin;
	};

	struct PointU16
	{
		uint16_t x, y;
	};

	void OutlineConvert(FT_Outline *outline, Outline *o, char c);

	void OutlineDecompose(FT_Outline *outline, Outline *o);

	void OutlineMakeCells(Outline *o);

	void OutlineSubdivide(Outline *o);

//  void OutlineFixCorners(Outline *o);

	void OutlineDestroy(Outline *o);

	void OutlineCbox(Outline *o, Rect *cbox);

	void OutlineU16Points(Outline *o, Rect *cbox, PointU16 *pout);

	template<typename T>
	static inline void DynArrayGrow(T **data, uint32_t *capacity, size_t element_size)
	{
		*capacity = *capacity ? *capacity * 2 : 8;
		T *new_data = (T *)realloc(*data, *capacity * element_size);
		//	assert(new_data);
		*data = new_data;
	}
}
