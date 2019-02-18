#include "Outline.hpp"

#include <stdlib.h>
#include <memory.h>
#include <cassert>
#include <vector>

namespace acid
{
	struct WIPCell
	{
		Rect bbox;
		uint32_t value;
		uint32_t from;
		uint32_t to;
		uint32_t start_len;
	};

	static void add_outline_point(Outline *o, Vector2 point)
	{
		if (o->point_capacity == o->num_of_points)
			dyn_array_grow(&o->points, &o->point_capacity, sizeof(Vector2));

		o->points[o->num_of_points] = point;
		//memcpy(o->points[o->num_of_points], point, sizeof(Vector2));
		o->num_of_points++;
	}

	static void add_outline_contour(Outline *o, ContourRange *range)
	{
		if (o->contour_capacity == o->num_of_contours)
			dyn_array_grow(&o->contours, &o->contour_capacity, sizeof(ContourRange));

		o->contours[o->num_of_contours] = *range;
		o->num_of_contours++;
	}

	static void outline_add_odd_point(Outline *o)
	{
		if (o->num_of_points % 2 != 0)
		{
			Vector2 p = {o->bbox.min_x, o->bbox.min_y};
			add_outline_point(o, p);
		}
	}

	static inline void convert_point(const FT_Vector *v, Vector2 &out)
	{
		out[0] = (float) v->x / 64.0f;
		out[1] = (float) v->y / 64.0f;
	}

	static int move_to_func(const FT_Vector *to, Outline *o)
	{
		Vector2 p = {0};

		if (o->num_of_contours > 0)
		{
			o->contours[o->num_of_contours - 1].end = o->num_of_points - 1;
			add_outline_point(o, p);
		}

		assert(o->num_of_points % 2 == 0);

		ContourRange range = {o->num_of_points, std::numeric_limits<uint32_t>::max()};
		add_outline_contour(o, &range);

		convert_point(to, p);
		add_outline_point(o, p);
		return 0;
	}

	static int line_to_func(const FT_Vector *to, Outline *o)
	{
		uint32_t last = o->num_of_points - 1;

		Vector2 to_p;
		convert_point(to, to_p);
		Vector2 p = o->points[last].Lerp(to_p, 0.5f);
		add_outline_point(o, p);
		add_outline_point(o, to_p);
		return 0;
	}

	static int conic_to_func(const FT_Vector *control, const FT_Vector *to, Outline *o)
	{
		Vector2 p;
		convert_point(control, p);
		add_outline_point(o, p);

		convert_point(to, p);
		add_outline_point(o, p);
		return 0;
	}

	static int cubic_to_func(const FT_Vector *control1, const FT_Vector *control2, const FT_Vector *to, Outline *o)
	{
		return line_to_func(to, o);
	}

	void outline_decompose(FT_Outline *outline, Outline *o)
	{
		memset(o, 0, sizeof(Outline));

		FT_BBox outline_bbox;
		FT_CHECK(FT_Outline_Get_BBox(outline, &outline_bbox));

		o->bbox.min_x = (float) outline_bbox.xMin / 64.0f;
		o->bbox.min_y = (float) outline_bbox.yMin / 64.0f;
		o->bbox.max_x = (float) outline_bbox.xMax / 64.0f;
		o->bbox.max_y = (float) outline_bbox.yMax / 64.0f;

		FT_Outline_Funcs funcs = {};
		funcs.move_to = reinterpret_cast<FT_Outline_MoveToFunc>(move_to_func);
		funcs.line_to = reinterpret_cast<FT_Outline_LineToFunc>(line_to_func);
		funcs.conic_to = reinterpret_cast<FT_Outline_ConicToFunc>(conic_to_func);
		funcs.cubic_to = reinterpret_cast<FT_Outline_CubicToFunc>(cubic_to_func);
			
		FT_CHECK(FT_Outline_Decompose(outline, &funcs, o));

		if (o->num_of_contours > 0)
		{
			o->contours[o->num_of_contours - 1].end = o->num_of_points - 1;
		}
	}

	static uint32_t cell_add_range(uint32_t cell, uint32_t from, uint32_t to)
	{
		assert(from % 2 == 0 && to % 2 == 0);

		from /= 2;
		to /= 2;

		if (from >= std::numeric_limits<uint8_t>::max()) return 0;
		if (to >= std::numeric_limits<uint8_t>::max()) return 0;

		uint32_t length = to - from;
		if (length <= 3 && (cell & 0x03) == 0)
		{
			cell |= from << 8;
			cell |= length;
			return cell;
		}

		if (length > 7)
			return 0;

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

// TODO: optimize
	static bool is_cell_filled(Outline *o, Rect *bbox)
	{
		Vector2 p = {
			(bbox->max_x + bbox->min_x) / 2.0f,
			(bbox->max_y + bbox->min_y) / 2.0f,
		};

		float mindist = std::numeric_limits<float>::max();
		float v = std::numeric_limits<float>::max();
		uint32_t j = std::numeric_limits<uint32_t>::max();

		for (uint32_t contour_index = 0; contour_index < o->num_of_contours; contour_index++)
		{
			uint32_t contour_begin = o->contours[contour_index].begin;
			uint32_t contour_end = o->contours[contour_index].end;

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];
				Vector2 &p2 = o->points[i + 2];

				float t = line_calculate_t(p0, p2, p);

				Vector2 p02 = p0.Lerp(p2, t);

				float udist = p02.Distance(p);

				if (udist < mindist + 0.0001f)
				{
					float d = line_signed_distance(p0, p2, p);

					if (udist >= mindist && i > contour_begin)
					{
						float lastd = i == contour_end - 2 && j == contour_begin
						              ? line_signed_distance(p0, p2, o->points[contour_begin + 2])
						              : line_signed_distance(p0, p2, o->points[i - 2]);

						if (lastd < 0.0) v = std::max(d, v);
						else v = std::min(d, v);
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

	static bool
	wipcell_add_bezier(Outline *o, Outline *u, uint32_t i, uint32_t j, uint32_t contour_index, WIPCell *cell)
	{
		bool ret = true;
		uint32_t ucontour_begin = u->contours[contour_index].begin;

		if (cell->to != std::numeric_limits<uint32_t>::max() && cell->to != j)
		{
			assert(cell->to < j);

			if (cell->from == ucontour_begin)
			{
				assert(cell->to % 2 == 0);
				assert(cell->from % 2 == 0);

				cell->start_len = (cell->to - cell->from) / 2;
			}
			else
			{
				cell->value = cell_add_range(cell->value, cell->from, cell->to);
				if (!cell->value) ret = false;
			}

			cell->from = j;
		}
		else
		{
			if (cell->from == std::numeric_limits<uint32_t>::max())
				cell->from = j;
		}

		cell->to = j + 2;
		return ret;
	}

	static bool
	wipcell_finish_contour(Outline *o, Outline *u, uint32_t contour_index, WIPCell *cell, uint32_t *max_start_len)
	{
		bool ret = true;
		uint32_t ucontour_begin = u->contours[contour_index].begin;
		uint32_t ucontour_end = u->contours[contour_index].end;

		if (cell->to < ucontour_end)
		{
			cell->value = cell_add_range(cell->value, cell->from, cell->to);
			if (!cell->value) ret = false;

			cell->from = std::numeric_limits<uint32_t>::max();
			cell->to = std::numeric_limits<uint32_t>::max();
		}

		assert(cell->to == std::numeric_limits<uint32_t>::max() || cell->to == ucontour_end);
		cell->to = std::numeric_limits<uint32_t>::max();

		if (cell->from != std::numeric_limits<uint32_t>::max() && cell->start_len != 0)
		{
			cell->value = cell_add_range(cell->value, cell->from, ucontour_end + cell->start_len * 2);
			if (!cell->value) ret = false;

			*max_start_len = std::max(*max_start_len, cell->start_len);
			cell->from = std::numeric_limits<uint32_t>::max();
			cell->start_len = 0;
		}

		if (cell->from != std::numeric_limits<uint32_t>::max())
		{
			cell->value = cell_add_range(cell->value, cell->from, ucontour_end);
			if (!cell->value) ret = false;

			cell->from = std::numeric_limits<uint32_t>::max();
		}

		if (cell->start_len != 0)
		{
			cell->value = cell_add_range(cell->value, ucontour_begin, ucontour_begin + cell->start_len * 2);
			if (!cell->value) ret = false;

			cell->start_len = 0;
		}

		assert(cell->from == std::numeric_limits<uint32_t>::max() && cell->to == std::numeric_limits<uint32_t>::max());
		return ret;
	}

	static bool
	for_each_wipcell_add_bezier(Outline *o, Outline *u, uint32_t i, uint32_t j, uint32_t contour_index, WIPCell *cells)
	{
		Rect bezier_bbox;
		bezier2_bbox(&o->points[i], &bezier_bbox);

		float outline_bbox_w = o->bbox.max_x - o->bbox.min_x;
		float outline_bbox_h = o->bbox.max_y - o->bbox.min_y;

		uint32_t min_x = (uint32_t) ((bezier_bbox.min_x - o->bbox.min_x) / outline_bbox_w * o->cell_count_x);
		uint32_t min_y = (uint32_t) ((bezier_bbox.min_y - o->bbox.min_y) / outline_bbox_h * o->cell_count_y);
		uint32_t max_x = (uint32_t) ((bezier_bbox.max_x - o->bbox.min_x) / outline_bbox_w * o->cell_count_x);
		uint32_t max_y = (uint32_t) ((bezier_bbox.max_y - o->bbox.min_y) / outline_bbox_h * o->cell_count_y);

		if (max_x >= o->cell_count_x) max_x = o->cell_count_x - 1;
		if (max_y >= o->cell_count_y) max_y = o->cell_count_y - 1;

		bool ret = true;
		for (uint32_t y = min_y; y <= max_y; y++)
		{
			for (uint32_t x = min_x; x <= max_x; x++)
			{
				WIPCell *cell = &cells[y * o->cell_count_x + x];
				if (bbox_bezier2_intersect(&cell->bbox, &o->points[i]))
					ret &= wipcell_add_bezier(o, u, i, j, contour_index, cell);
			}
		}

		return ret;
	}

	static bool for_each_wipcell_finish_contour(Outline *o, Outline *u, uint32_t contour_index, WIPCell *cells,
	                                            uint32_t *max_start_len)
	{
		bool ret = true;
		for (uint32_t y = 0; y < o->cell_count_y; y++)
		{
			for (uint32_t x = 0; x < o->cell_count_x; x++)
			{
				WIPCell *cell = &cells[y * o->cell_count_x + x];
				ret &= wipcell_finish_contour(o, u, contour_index, cell, max_start_len);
			}
		}

		return ret;
	}

	static void copy_wipcell_values(Outline *u, WIPCell *cells)
	{
		u->cells = (uint32_t *) malloc(sizeof(uint32_t) * u->cell_count_x * u->cell_count_y);

		for (uint32_t y = 0; y < u->cell_count_y; y++)
		{
			for (uint32_t x = 0; x < u->cell_count_x; x++)
			{
				uint32_t i = y * u->cell_count_x + x;
				u->cells[i] = cells[i].value;
			}
		}

	}

	static void init_wipcells(Outline *o, WIPCell *cells)
	{
		float w = o->bbox.max_x - o->bbox.min_x;
		float h = o->bbox.max_y - o->bbox.min_y;

		for (uint32_t y = 0; y < o->cell_count_y; y++)
		{
			for (uint32_t x = 0; x < o->cell_count_x; x++)
			{
				Rect bbox = {
					o->bbox.min_x + ((float) x / o->cell_count_x) * w,
					o->bbox.min_y + ((float) y / o->cell_count_y) * h,
					o->bbox.min_x + ((float) (x + 1) / o->cell_count_x) * w,
					o->bbox.min_y + ((float) (y + 1) / o->cell_count_y) * h,
				};

				uint32_t i = y * o->cell_count_x + x;
				cells[i].bbox = bbox;
				cells[i].from = std::numeric_limits<uint32_t>::max();
				cells[i].to = std::numeric_limits<uint32_t>::max();
				cells[i].value = 0;
				cells[i].start_len = 0;
			}
		}
	}

	static uint32_t outline_add_filled_line(Outline *o)
	{
		outline_add_odd_point(o);

		uint32_t i = o->num_of_points;
		float y = o->bbox.max_y + 1000.0f;
		Vector2 f0 = {o->bbox.min_x, y};
		Vector2 f1 = {o->bbox.min_x + 10.0f, y};
		Vector2 f2 = {o->bbox.min_x + 20.0f, y};
		add_outline_point(o, f0);
		add_outline_point(o, f1);
		add_outline_point(o, f2);

		return i;
	}

	static uint32_t make_cell_from_single_edge(uint32_t e)
	{
		assert(e % 2 == 0);
		return e << 7 | 1;
	}

	static void set_filled_cells(Outline *u, WIPCell *cells, uint32_t filled_cell)
	{
		for (uint32_t y = 0; y < u->cell_count_y; y++)
		{
			for (uint32_t x = 0; x < u->cell_count_x; x++)
			{
				uint32_t i = y * u->cell_count_x + x;
				WIPCell *cell = &cells[i];

				if (cell->value == 0 && is_cell_filled(u, &cell->bbox))
					cell->value = filled_cell;
			}
		}
	}

	static bool try_to_fit_in_cell_count(Outline *o)
	{
		bool ret = true;

		auto cells = std::vector<WIPCell>(o->cell_count_x * o->cell_count_y);
		init_wipcells(o, cells.data());

		Outline u = {};
		u.bbox = o->bbox;
		u.cell_count_x = o->cell_count_x;
		u.cell_count_y = o->cell_count_y;

		for (uint32_t contour_index = 0; contour_index < o->num_of_contours; contour_index++)
		{
			uint32_t contour_begin = o->contours[contour_index].begin;
			uint32_t contour_end = o->contours[contour_index].end;

			outline_add_odd_point(&u);

			ContourRange urange = {u.num_of_points, u.num_of_points + contour_end - contour_begin};
			add_outline_contour(&u, &urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];
				//Vector2 &p2 = o->points[i + 2];

				uint32_t j = u.num_of_points;
				add_outline_point(&u, p0);
				add_outline_point(&u, p1);

				ret &= for_each_wipcell_add_bezier(o, &u, i, j, contour_index, cells.data());
			}

			uint32_t max_start_len = 0;
			ret &= for_each_wipcell_finish_contour(o, &u, contour_index, cells.data(), &max_start_len);

			uint32_t continuation_end = contour_begin + max_start_len * 2;
			for (uint32_t i = contour_begin; i < continuation_end; i += 2)
			{
				add_outline_point(&u, o->points[i]);
				add_outline_point(&u, o->points[i + 1]);
			}

			Vector2 &plast = o->points[continuation_end];
			add_outline_point(&u, plast);
		}

		if (!ret)
		{
			outline_destroy(&u);
			return ret;
		}

		uint32_t filled_line = outline_add_filled_line(&u);
		uint32_t filled_cell = make_cell_from_single_edge(filled_line);
		set_filled_cells(&u, cells.data(), filled_cell);

		copy_wipcell_values(&u, cells.data());

		outline_destroy(o);
		*o = u;
		return ret;
	}

	static uint32_t uint32_to_pow2(uint32_t v)
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

	void outline_make_cells(Outline *o)
	{
		if (o->num_of_points > FD_OUTLINE_MAX_POINTS)
			return;

		float w = o->bbox.max_x - o->bbox.min_x;
		float h = o->bbox.max_y - o->bbox.min_y;

		float multiplier = 0.5f;
		if (h > w * 1.8f || w > h * 1.8f)
			multiplier = 1.0f;

		uint32_t c = uint32_to_pow2((uint32_t) sqrtf(o->num_of_points * 0.75f));

		o->cell_count_x = c;
		o->cell_count_y = c;

		if (h > w * 1.8f) o->cell_count_x /= 2;
		if (w > h * 1.8f) o->cell_count_y /= 2;

		while (true)
		{
			if (try_to_fit_in_cell_count(o))
				break;

			if (o->cell_count_x > 64 || o->cell_count_y > 64)
			{
				o->cell_count_x = 0;
				o->cell_count_y = 0;
				return;
			}

			if (o->cell_count_x == o->cell_count_y)
			{
				if (w > h) o->cell_count_x *= 2;
				else o->cell_count_y *= 2;
			}
			else
			{
				if (o->cell_count_x < o->cell_count_y)
					o->cell_count_x *= 2;
				else o->cell_count_y *= 2;
			}
		}
	}

/*
void outline_fix_corners(Outline *o)
{
	float fix_dist = 0.001f;

	for (uint32_t contour_index = 0; contour_index < o->num_of_contours; contour_index++)
	{
		uint32_t contour_begin = o->contours[contour_index].begin;
		uint32_t contour_end = o->contours[contour_index].end;

		for (uint32_t i = contour_begin; i < contour_end; i += 2)
		{
			uint32_t prev = i - 1;
			if (contour_begin == i)
				prev = contour_end - 1;

			float *r = o->points[prev];
			float *p0 = o->points[i];
			float *p1 = o->points[i + 1];

			Vector2 v0, v1;
			Vector2_sub(v0, r, p0);
			Vector2_sub(v1, p1, p0);

			Vector2_norm_(v0);
			Vector2_norm_(v1);

			float angle = acosf(Vector2_dot(v0, v1));
			if (angle <= M_PI / 2.0f * 1.025f)
			{
				Vector2_scale_(v0, fix_dist);
				Vector2_scale_(v1, fix_dist);

				Vector2 f0, f1;
				Vector2_sub(f1, p0, v0);
				Vector2_sub(f0, p0, v1);

				outline_add_odd_point(o);

				if (o->corner_fix_begin == 0)
					o->corner_fix_begin = o->num_of_points;

				add_outline_point(o, f0);
				add_outline_point(o, f1);
			}
		}
	}
}
*/

	void outline_subdivide(Outline *o)
	{
		Outline u = {};
		u.bbox = o->bbox;		

		for (uint32_t contour_index = 0; contour_index < o->num_of_contours; contour_index++)
		{
			uint32_t contour_begin = o->contours[contour_index].begin;
			uint32_t contour_end = o->contours[contour_index].end;

			outline_add_odd_point(&u);

			ContourRange urange = {u.num_of_points, std::numeric_limits<uint32_t>::max()};
			add_outline_contour(&u, &urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 &p0 = o->points[i];
				//Vector2 &p1 = o->points[i + 1];
				//Vector2 &p2 = o->points[i + 2];

				Vector2 newp[3];
				bezier2_split_3p(newp, &o->points[i], 0.5f);

				add_outline_point(&u, p0);
				add_outline_point(&u, newp[0]);
				add_outline_point(&u, newp[1]);
				add_outline_point(&u, newp[2]);
			}

			u.contours[contour_index].end = u.num_of_points;
			add_outline_point(&u, o->points[contour_end]);
		}

		outline_destroy(o);
		*o = u;
	}

// TODO: optimize
	void outline_fix_thin_lines(Outline *o)
	{
		Outline u = {};
		u.bbox = o->bbox;		

		for (uint32_t contour_index = 0; contour_index < o->num_of_contours; contour_index++)
		{
			uint32_t contour_begin = o->contours[contour_index].begin;
			uint32_t contour_end = o->contours[contour_index].end;

			outline_add_odd_point(&u);

			ContourRange urange = {u.num_of_points, std::numeric_limits<uint32_t>::max()};
			add_outline_contour(&u, &urange);

			for (uint32_t i = contour_begin; i < contour_end; i += 2)
			{
				Vector2 &p0 = o->points[i];
				Vector2 &p1 = o->points[i + 1];
				Vector2 &p2 = o->points[i + 2];

				Vector2 mid = p0.Lerp(p2, 0.5f);
				Vector2 midp1 = p1 - mid;

				Vector2 bezier[] = {
					{p0[0], p0[1]},
					{p1[0], p1[1]},
					{p2[0], p2[1]}
				};

				bezier[1] += midp1;
				/*
				bool subdivide = false;
				if (i > 2)
				{
					uint32_t jbegin = contour_begin;
					if (i == contour_end - 2) jbegin += 2;

					for (uint32_t j = jbegin; j < i - 2; j += 2)
					{
						float *q0 = o->points[j];
						float *q2 = o->points[j + 2];

						if (bezier2_line_is_intersecting(bezier, q0, q2))
							subdivide = true;
					}
				}

				uint32_t jend = contour_end;
				if (i == contour_begin) jend -= 2;

				for (uint32_t j = i + 2; j < jend; j += 2)
				{
					float *q0 = o->points[j];
					float *q2 = o->points[j + 2];

					if (bezier2_line_is_intersecting(bezier, q0, q2))
						subdivide = true;
				}
				*/
				bool subdivide = false;
				for (uint32_t j = contour_begin; j < contour_end; j += 2)
				{
					if (i == contour_begin && j == contour_end - 2) continue;
					if (i == contour_end - 2 && j == contour_begin) continue;
					if (j + 2 >= i && j <= i + 2) continue;

					Vector2 &q0 = o->points[j];
					//Vector2 &q1 = o->points[j + 1];
					Vector2 &q2 = o->points[j + 2];

					if (bezier2_line_is_intersecting(bezier, q0, q2))
						subdivide = true;
				}

				if (subdivide)
				{
					Vector2 newp[3];
					bezier2_split_3p(newp, &o->points[i], 0.5f);

					add_outline_point(&u, p0);
					add_outline_point(&u, newp[0]);
					add_outline_point(&u, newp[1]);
					add_outline_point(&u, newp[2]);
				}
				else
				{
					add_outline_point(&u, p0);
					add_outline_point(&u, p1);
				}
			}

			u.contours[contour_index].end = u.num_of_points;
			add_outline_point(&u, o->points[contour_end]);
		}

		outline_destroy(o);
		*o = u;
	}

	void outline_convert(FT_Outline *outline, Outline *o, char c)
	{
		if (c == '&')
		{
			printf("");
		}
		/*
		clock_t t = clock();
		for (int i = 0; i < 1000; i++)
		{*/
		outline_decompose(outline, o);
		//outline_fix_corners(o);
		//outline_subdivide(o);
		outline_fix_thin_lines(o);
		outline_make_cells(o);
		//}

		//printf("  %d ms\n", clock() - t);
	}

	void outline_destroy(Outline *o)
	{
		if (o->contours) free(o->contours);
		if (o->points) free(o->points);
		if (o->cells) free(o->cells);
		memset(o, 0, sizeof(Outline));
	}

	void outline_cbox(Outline *o, Rect *cbox)
	{
		if (o->num_of_points == 0)
			return;

		cbox->min_x = o->points[0][0];
		cbox->min_y = o->points[0][1];
		cbox->max_x = o->points[0][0];
		cbox->max_y = o->points[0][1];

		for (uint32_t i = 1; i < o->num_of_points; i++)
		{
			float x = o->points[i][0];
			float y = o->points[i][1];

			cbox->min_x = std::min(cbox->min_x, x);
			cbox->min_y = std::min(cbox->min_y, y);
			cbox->max_x = std::max(cbox->max_x, x);
			cbox->max_y = std::max(cbox->max_y, y);
		}
	}

	static inline uint16_t gen_u16_value(float x, float min, float max)
	{
		return (uint16_t) ((x - min) / (max - min) * std::numeric_limits<uint16_t>::max());
	}

	void outline_u16_points(Outline *o, Rect *cbox, PointU16 *pout)
	{
		outline_cbox(o, cbox);

		for (uint32_t i = 0; i < o->num_of_points; i++)
		{
			float x = o->points[i][0];
			float y = o->points[i][1];

			pout[i].x = gen_u16_value(x, cbox->min_x, cbox->max_x);
			pout[i].y = gen_u16_value(y, cbox->min_y, cbox->max_y);
		}
	}
}
