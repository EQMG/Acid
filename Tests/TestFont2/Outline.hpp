#pragma once

#include <stdint.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BBOX_H
#include FT_OUTLINE_H

#include "Geometry.hpp"

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

void outline_convert(FT_Outline *outline, Outline *o, char c);
void outline_decompose(FT_Outline *outline, Outline *o);
void outline_make_cells(Outline *o);
void outline_subdivide(Outline *o);
//void outline_fix_corners(Outline *o);
void outline_destroy(Outline *o);
void outline_cbox(Outline *o, Rect *cbox);
void outline_u16_points(Outline *o, Rect *cbox, PointU16 *pout);

template <typename T>
static inline void dyn_array_grow(T **data, uint32_t *capacity, size_t element_size)
{
	*capacity = *capacity ? *capacity * 2 : 8;
	T * new_data = (T*) realloc(*data, *capacity * element_size);
//	assert(new_data);
	*data = new_data;
}
