#include "msdf.h"

#include "stb_truetype.h"

// pixel at (x, y) in bitmap (arr)
#define P(x, y, w, arr) ((vec3){arr[(3*(((y)*w)+x))], arr[(3*(((y)*w)+x))+1], arr[(3*(((y)*w)+x))+2]})

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define INF   -1e24
#define RANGE 1.0
#define EDGE_THRESHOLD 0.02

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef float vec2[2];
typedef float vec3[3];

typedef struct {
  double dist;
  double d;
} signed_distance_t;

// the possible types:
// STBTT_vmove  = start of a contour
// STBTT_vline  = linear segment
// STBTT_vcurve = quadratic segment
// STBTT_vcubic = cubic segment
typedef struct {
  int color;
  vec2 p[4];
  int type;
} edge_segment_t;

// defines what color channel an edge belongs to
typedef enum {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7
} edge_color_t;

double median(double a, double b, double c)
{
  return MAX(MIN(a, b), MIN(MAX(a, b), c));
}

int nonzero_sign(double n)
{
  return 2*(n > 0)-1;
}

double cross(vec2 a, vec2 b)
{
  return a[0]*b[1] - a[1]*b[0];
}

void vec2_scale(vec2 r, vec2 const v, float const s)
{
  int i;
  for(i=0; i<2; ++i)
    r[i] = v[i] * s;
}

float vec2_mul_inner(vec2 const a, vec2 const b)
{
  float p = 0.;
  int i;
  for(i=0; i<2; ++i)
    p += b[i]*a[i];
  return p;
}

float vec2_len(vec2 const v)
{
  return sqrtf(vec2_mul_inner(v,v));
}

void vec2_norm(vec2 r, vec2 const v)
{
  float k = 1.0 / vec2_len(v);
  vec2_scale(r, v, k);
}

void vec2_sub(vec2 r, vec2 const a, vec2 const b)
{
  int i;
  for(i=0; i<2; ++i)
    r[i] = a[i] - b[i];
}

int solve_quadratic(double x[2], double a, double b, double c)
{
  if (fabs(a) < 1e-14) {
    if (fabs(b) < 1e-14) {
      if (c == 0)
        return -1;
      return 0;
    }
    x[0] = -c/b;
    return 1;
  }

  double dscr = b*b-4*a*c;
  if (dscr > 0) {
    dscr = sqrt(dscr);
    x[0] = (-b+dscr)/(2*a);
    x[1] = (-b-dscr)/(2*a);
    return 2;
  } else if (dscr == 0) {
    x[0] = -b/(2*a);
    return 1;
  } else {
    return 0;
  }
}

int solve_cubic_normed(double *x, double a, double b, double c)
{
  double a2 = a*a;
  double q  = (a2 - 3*b)/9; 
  double r  = (a*(2*a2-9*b) + 27*c)/54;
  double r2 = r*r;
  double q3 = q*q*q;
  double A, B;
  if (r2 < q3) {
    double t = r/sqrt(q3);
    if (t < -1) t = -1;
    if (t > 1) t = 1;
    t = acos(t);
    a /= 3; q = -2*sqrt(q);
    x[0] = q*cos(t/3)-a;
    x[1] = q*cos((t+2*M_PI)/3)-a;
    x[2] = q*cos((t-2*M_PI)/3)-a;
    return 3;
  } else {
    A = -pow(fabs(r)+sqrt(r2-q3), 1/3.); 
    if (r < 0) A = -A;
    B = A == 0 ? 0 : q/A;
    a /= 3;
    x[0] = (A+B)-a;
    x[1] = -0.5*(A+B)-a;
    x[2] = 0.5*sqrt(3.)*(A-B);
    if (fabs(x[2]) < 1e-14)
      return 2;
    return 1;
  }
}

int solve_cubic(double x[3], double a, double b, double c, double d)
{
  if (fabs(a) < 1e-14)
    return solve_quadratic(x, b, c, d);
  
  return solve_cubic_normed(x, b/a, c/a, d/a);
}

void getortho(vec2 r, vec2 const v, int polarity, int allow_zero)
{
  double len = vec2_len(v);
  
  if (len == 0) {
    if (polarity) {
      r[0] = 0;
      r[1] = !allow_zero;
    } else {
      r[0] = 0;
      r[1] = -!allow_zero;
    }
    return;
  }
  
  if (polarity) {
    r[0] = -v[1]/len;
    r[1] = v[0]/len;
  } else {
    r[0] = v[1]/len;
    r[1] = -v[0]/len;
  }
}

int pixel_clash(const vec3 a, const vec3 b, double threshold)
{
  int aIn = (a[0] > .5f)+(a[1] > .5f)+(a[2] > .5f) >= 2;
  int bIn = (b[0] > .5f)+(b[1] > .5f)+(b[2] > .5f) >= 2;
  if (aIn != bIn) return 0;
  if ((a[0] > .5f && a[1] > .5f && a[2] > .5f) || (a[0] < .5f && a[1] < .5f && a[2] < .5f)
      || (b[0] > .5f && b[1] > .5f && b[2] > .5f) || (b[0] < .5f && b[1] < .5f && b[2] < .5f))
    return 0;
  float aa, ab, ba, bb, ac, bc;
  if ((a[0] > .5f) != (b[0] > .5f) && (a[0] < .5f) != (b[0] < .5f)) {
    aa = a[0], ba = b[0];
    if ((a[1] > .5f) != (b[1] > .5f) && (a[1] < .5f) != (b[1] < .5f)) {
      ab = a[1], bb = b[1];
      ac = a[2], bc = b[2];
    } else if ((a[2] > .5f) != (b[2] > .5f) && (a[2] < .5f) != (b[2] < .5f)) {
      ab = a[2], bb = b[2];
      ac = a[1], bc = b[1];
    } else {
      return 0;
    }
  } else if ((a[1] > .5f) != (b[1] > .5f) && (a[1] < .5f) != (b[1] < .5f)
      && (a[2] > .5f) != (b[2] > .5f) && (a[2] < .5f) != (b[2] < .5f)) {
    aa = a[1], ba = b[1];
    ab = a[2], bb = b[2];
    ac = a[0], bc = b[0];
  } else {
    return 0;
  }
  return (fabsf(aa-ba) >= threshold)
      && (fabsf(ab-bb) >= threshold)
      && fabsf(ac-.5f) >= fabsf(bc-.5f);
}

void mix(vec2 r, vec2 a, vec2 b, double weight)
{ 
  r[0] = (1-weight)*a[0]+weight*b[0];
  r[1] = (1-weight)*a[1]+weight*b[1];
}

void linear_direction(vec2 r, edge_segment_t *e, double param)
{
  r[0] = e->p[1][0] - e->p[0][0];
  r[1] = e->p[1][1] - e->p[0][1];
}

void quadratic_direction(vec2 r, edge_segment_t *e, double param)
{
  vec2 a,b;
  vec2_sub(a, e->p[1], e->p[0]);
  vec2_sub(b, e->p[2], e->p[1]);
  mix(r, a, b, param);
}

void cubic_direction(vec2 r, edge_segment_t *e, double param)
{
  vec2 a,b,c,d,t;
  vec2_sub(a, e->p[1], e->p[0]);
  vec2_sub(b, e->p[2], e->p[1]);
  mix(c, a, b, param);
  vec2_sub(a, e->p[3], e->p[2]);
  mix(d, b, a, param);
  mix(t, c, d, param);

  if (!t[0] && !t[1]) {
    if (param == 0) {
      r[0] = e->p[2][0] - e->p[0][0];
      r[1] = e->p[2][1] - e->p[0][1];
      return;
    }
    if (param == 1) {
      r[0] = e->p[3][0] - e->p[1][0];
      r[1] = e->p[3][1] - e->p[1][1];
      return;
    }
  }

  r[0] = t[0];
  r[1] = t[1];
}

void direction(vec2 r, edge_segment_t *e, double param)
{
  switch (e->type) {
    case STBTT_vline: {
      linear_direction(r, e, param);
      break;
    }
    case STBTT_vcurve: {
      quadratic_direction(r, e, param);
      break;
    }
    case STBTT_vcubic: {
      cubic_direction(r, e, param);
      break;
    }
  }
}

void linear_point(vec2 r, edge_segment_t *e, double param)
{
  mix(r, e->p[0], e->p[1], param);
}

void quadratic_point(vec2 r, edge_segment_t *e, double param)
{
  vec2 a,b;
  mix(a, e->p[0], e->p[1], param);
  mix(b, e->p[1], e->p[2], param);
  mix(r, a, b, param);
}

void cubic_point(vec2 r, edge_segment_t *e, double param)
{
  vec2 p12, a, b, c, d;
  mix(p12, e->p[1], e->p[2], param);

  mix(a, e->p[0], e->p[1], param);
  mix(b, a, p12, param);

  mix(c, e->p[2], e->p[3], param);
  mix(d, p12, c, param);

  mix(r, b, d, param);
}

void point(vec2 r, edge_segment_t *e, double param)
{
  switch (e->type) {
    case STBTT_vline: {
      linear_point(r, e, param);
      break;
    }
    case STBTT_vcurve: {
      quadratic_point(r, e, param);
      break;
    }
    case STBTT_vcubic: {
      cubic_point(r, e, param);
      break;
    }
  }
}

// linear edge signed distance 
signed_distance_t linear_dist(edge_segment_t *e, vec2 origin, double *param)
{
  vec2 aq, ab, eq;
  vec2_sub(aq, origin, e->p[0]);
  vec2_sub(ab, e->p[1], e->p[0]);
  *param = vec2_mul_inner(aq, ab) / vec2_mul_inner(ab, ab);
  vec2_sub(eq, e->p[*param > .5], origin);

  double endpoint_distance = vec2_len(eq);
  if (*param > 0 && *param < 1) {
    vec2 ab_ortho;
    getortho(ab_ortho, ab, 0 , 0);
    double ortho_dist = vec2_mul_inner(ab_ortho, aq);
    if (fabs(ortho_dist) < endpoint_distance)
      return (signed_distance_t){ortho_dist, 0};
  }

  vec2_norm(ab, ab);
  vec2_norm(eq, eq);
  double dist = nonzero_sign(cross(aq, ab)) * endpoint_distance;
  double d    = fabs(vec2_mul_inner(ab, eq));
  return (signed_distance_t){dist, d};
}

// quadratic edge signed distance
signed_distance_t quadratic_dist(edge_segment_t *e, vec2 origin, double *param)
{
  vec2 qa, ab, br;
  vec2_sub(qa, e->p[0], origin);
  vec2_sub(ab, e->p[1], e->p[0]);
  br[0] = e->p[0][0] + e->p[2][0] - e->p[1][0] - e->p[1][0];
  br[1] = e->p[0][1] + e->p[2][1] - e->p[1][1] - e->p[1][1];

  double a = vec2_mul_inner(br, br);
  double b = 3*vec2_mul_inner(ab, br);
  double c = 2*vec2_mul_inner(ab, ab)+vec2_mul_inner(qa, br);
  double d = vec2_mul_inner(qa, ab);
  double t[3];
  int solutions = solve_cubic(t, a, b, c, d);

  // distance from a
  double min_distance = nonzero_sign(cross(ab, qa))*vec2_len(qa);
  *param = -vec2_mul_inner(qa, ab)  / vec2_mul_inner(ab, ab);
  {
    vec2 a,b;
    vec2_sub(a, e->p[2], e->p[1]);
    vec2_sub(b, e->p[2], origin);

    // distance from b
    double distance = nonzero_sign(cross(a, b))*vec2_len(b);
    if (fabs(distance) < fabs(min_distance)) {
      min_distance = distance;

      vec2_sub(a, origin, e->p[1]);
      vec2_sub(b, e->p[2], e->p[1]);
      *param = vec2_mul_inner(a, b) / vec2_mul_inner(b, b);
    }
  }

  for (int i=0; i<solutions; ++i) {
    if (t[i] > 0 && t[i] < 1) {
      // end_point = p[0]+2*t[i]*ab+t[i]*t[i]*br;
      vec2 end_point, a, b;
      end_point[0] = e->p[0][0]+2*t[i]*ab[0]+t[i]*t[i]*br[0];
      end_point[1] = e->p[0][1]+2*t[i]*ab[1]+t[i]*t[i]*br[1];
      
      vec2_sub(a, e->p[2], e->p[0]);
      vec2_sub(b, end_point, origin);
      double distance = nonzero_sign(cross(a, b))*vec2_len(b);
      if (fabs(distance) <= fabs(min_distance)) {
        min_distance = distance;
        *param = t[i];
      }
    }
  }

  if (*param >= 0 && *param <= 1)
    return (signed_distance_t){min_distance, 0};

  vec2 aa,bb;
  vec2_norm(ab, ab);
  vec2_norm(qa, qa);
  vec2_sub(aa, e->p[2], e->p[1]);
  vec2_norm(aa, aa);
  vec2_sub(bb, e->p[2], origin);
  vec2_norm(bb, bb);

  if (*param < .5)
    return (signed_distance_t){min_distance, fabs(vec2_mul_inner(ab, qa))};
  else
    return (signed_distance_t){min_distance, fabs(vec2_mul_inner(aa, bb))};
}

// cubic edge signed distance
signed_distance_t cubic_dist(edge_segment_t *e, vec2 origin, double *param)
{
  vec2 qa, ab, br, as;
  vec2_sub(qa, e->p[0], origin);
  vec2_sub(ab, e->p[1], e->p[0]);
  br[0] = e->p[2][0] - e->p[1][0] - ab[0];
  br[1] = e->p[2][1] - e->p[1][1] - ab[1];
  as[0] = (e->p[3][0] - e->p[2][0]) - (e->p[2][0] - e->p[1][0]) - br[0];
  as[1] = (e->p[3][1] - e->p[2][1]) - (e->p[2][1] - e->p[1][1]) - br[1];

  vec2 ep_dir;
  direction(ep_dir, e, 0);

  // distance from a
  double min_distance = nonzero_sign(cross(ep_dir, qa))*vec2_len(qa);
  *param = -vec2_mul_inner(qa, ep_dir)  / vec2_mul_inner(ep_dir, ep_dir);
  {
    vec2 a;
    vec2_sub(a, e->p[3], origin);

    direction(ep_dir, e, 1);
    // distance from b
    double distance = nonzero_sign(cross(ep_dir, a))*vec2_len(a);
    if (fabs(distance) < fabs(min_distance)) {
      min_distance = distance;

      a[0] = origin[0] + ep_dir[0] - e->p[3][0];
      a[1] = origin[1] + ep_dir[1] - e->p[3][1];
      *param = vec2_mul_inner(a, ep_dir) / vec2_mul_inner(ep_dir, ep_dir);
    }
  }

  const int search_starts = 4;
  for (int i=0; i<=search_starts; ++i) {
    double t = (double)i/search_starts;
    for (int step=0;; ++step) {
      vec2 qpt;
      point(qpt, e, t);
      vec2_sub(qpt, qpt, origin);
      vec2 d;
      direction(d, e, t);
      double distance = nonzero_sign(cross(d, qpt))*vec2_len(qpt);
      if (fabs(distance) < fabs(min_distance)) {
        min_distance = distance;
        *param = t;
      }
      if (step == search_starts)
        break;
      
      vec2 d1,d2;
      d1[0] = 3*as[0]*t*t+6*br[0]*t+3*ab[0];
      d1[1] = 3*as[1]*t*t+6*br[1]*t+3*ab[1];
      d2[0] = 6*as[0]*t+6*br[0];
      d2[1] = 6*as[1]*t+6*br[1];

      t -= vec2_mul_inner(qpt, d1) / (vec2_mul_inner(d1, d1)+vec2_mul_inner(qpt, d2));
      if (t < 0 || t > 1)
        break;
    }
  }

  if (*param >= 0 && *param <= 1)
    return (signed_distance_t){min_distance, 0};

  vec2 d0, d1;
  direction(d0, e, 0);
  direction(d1, e, 1);
  vec2_norm(d0, d0);
  vec2_norm(d1, d1);
  vec2_norm(qa, qa);
  vec2 a;
  vec2_sub(a, e->p[3], origin);
  vec2_norm(a, a);

  if (*param < .5)
    return (signed_distance_t){min_distance, fabs(vec2_mul_inner(d0, qa))};
  else
    return (signed_distance_t){min_distance, fabs(vec2_mul_inner(d1, a))};
}

void dist_to_pseudo(signed_distance_t *distance, vec2 origin, double param, edge_segment_t *e)
{
  if (param < 0) {
    vec2 dir, p;
    direction(dir, e, 0);
    vec2_norm(dir, dir);
    vec2 aq  = {origin[0], origin[1]};
    point(p, e, 0);
    vec2_sub(aq, origin, p);
    double ts = vec2_mul_inner(aq, dir);
    if (ts < 0) {
      double pseudo_dist = cross(aq, dir);
      if (fabs(pseudo_dist) <= fabs(distance->dist)) {
        distance->dist = pseudo_dist;
        distance->d    = 0;
      }
    }
  } else if (param > 1) {
    vec2 dir,p;
    direction(dir, e, 1);
    vec2_norm(dir, dir);
    vec2 bq  = {origin[0], origin[1]};
    point(p, e, 1);
    vec2_sub(bq, origin, p);
    double ts = vec2_mul_inner(bq, dir);
    if (ts > 0) {
      double pseudo_dist = cross(bq, dir);
      if (fabs(pseudo_dist) <= fabs(distance->dist)) {
        distance->dist = pseudo_dist;
        distance->d    = 0;
      }
    }
  }
}

int signed_compare(signed_distance_t a, signed_distance_t b)
{
  return fabs(a.dist) < fabs(b.dist) || (fabs(a.dist) == fabs(b.dist) && a.d < b.d);
}

int is_corner(vec2 a, vec2 b, double threshold)
{
  return vec2_mul_inner(a, b) <= 0 || fabs(cross(a, b)) > threshold;
}

void switch_color(edge_color_t *color, unsigned long long *seed, edge_color_t banned)
{
  edge_color_t combined = *color & banned;
  if (combined == RED || combined == GREEN || combined == BLUE) {
    *color = (edge_color_t)(combined ^ WHITE);
    return;
  }

  if (*color == BLACK || *color == WHITE) {
    static const edge_color_t start[3] = {CYAN, MAGENTA, YELLOW};
    *color = start[*seed & 3];
    *seed /= 3;
    return;
  }

  int shifted = *color<<(1+(*seed&1));
  *color = (edge_color_t)((shifted|shifted>>3)&WHITE);
  *seed >>= 1;
}

void linear_split(edge_segment_t *e, edge_segment_t *p1, edge_segment_t *p2, edge_segment_t *p3)
{
  vec2 p;
  
  point(p, e, 1/3.0);
  memcpy(&p1->p[0], e->p[0] , sizeof(vec2));
  memcpy(&p1->p[1], p, sizeof(vec2));
  p1->color = e->color;
  
  point(p, e, 1/3.0);
  memcpy(&p2->p[0], p , sizeof(vec2));
  point(p, e, 2/3.0);
  memcpy(&p2->p[1], p, sizeof(vec2));
  p2->color = e->color;

  point(p, e, 2/3.0);
  memcpy(&p3->p[0], p , sizeof(vec2));
  point(p, e, 2/3.0);
  memcpy(&p3->p[1], e->p[1], sizeof(vec2));
  p3->color = e->color;
}

void quadratic_split(edge_segment_t *e, edge_segment_t *p1, edge_segment_t *p2, edge_segment_t *p3)
{
  vec2 p,a,b;
  
  memcpy(&p1->p[0], e->p[0] , sizeof(vec2));
  mix(p, e->p[0], e->p[1], 1/3.0);
  memcpy(&p1->p[1], p, sizeof(vec2));
  point(p, e, 1/3.0);
  memcpy(&p1->p[2], p, sizeof(vec2));
  p1->color = e->color;
  
  point(p, e, 1/3.0);
  memcpy(&p2->p[0], p , sizeof(vec2));
  mix(a, e->p[0], e->p[1], 5/9.0);
  mix(b, e->p[1], e->p[2], 4/9.0);
  mix(p, a, b, 0.5);
  memcpy(&p2->p[1], p, sizeof(vec2));
  point(p, e, 2/3.0);
  memcpy(&p2->p[2], p, sizeof(vec2));
  p2->color = e->color;

  point(p, e, 2/3.0);
  memcpy(&p3->p[0], p , sizeof(vec2));
  mix(p, e->p[1], e->p[2], 2/3.0);
  memcpy(&p3->p[1], p, sizeof(vec2));
  memcpy(&p3->p[2], e->p[2], sizeof(vec2));
  p3->color = e->color;
}

void cubic_split(edge_segment_t *e, edge_segment_t *p1, edge_segment_t *p2, edge_segment_t *p3)
{
  vec2 p,a,b,c,d;
  
  memcpy(&p1->p[0], e->p[0], sizeof(vec2)); // p1 0
  if (e->p[0] == e->p[1]) {
    memcpy(&p1->p[1], e->p[0], sizeof(vec2)); // ? p1 1
  } else {
    mix(p, e->p[0], e->p[1], 1/3.0);
    memcpy(&p1->p[1], p, sizeof(vec2)); // ? p1 1
  }
  mix(a, e->p[0], e->p[1], 1/3.0);
  mix(b, e->p[1], e->p[2], 1/3.0);
  mix(p, a, b, 1/3.0);
  memcpy(&p1->p[2], p, sizeof(vec2)); // p1 2
  point(p, e, 1/3.0);
  memcpy(&p1->p[3], p, sizeof(vec2)); // p1 3
  p1->color = e->color;

  point(p, e, 1/3.0);
  memcpy(&p2->p[0], p, sizeof(vec2)); // p2 0
  mix(a, e->p[0], e->p[1], 1/3.0);
  mix(b, e->p[1], e->p[2], 1/3.0);
  mix(c, a, b, 1/3.0);
  mix(a, e->p[1], e->p[2], 1/3.0);
  mix(b, e->p[2], e->p[3], 1/3.0);
  mix(d, a, b, 1/3.0);
  mix(p, c, d, 2/3.0);
  memcpy(&p2->p[1], p, sizeof(vec2)); // p2 1
  mix(a, e->p[0], e->p[1], 2/3.0);
  mix(b, e->p[1], e->p[2], 2/3.0);
  mix(c, a, b, 2/3.0);
  mix(a, e->p[1], e->p[2], 2/3.0);
  mix(b, e->p[2], e->p[3], 2/3.0);
  mix(d, a, b, 2/3.0);
  mix(p, c, d, 1/3.0);
  memcpy(&p2->p[2], p, sizeof(vec2)); // p2 2
  point(p, e, 2/3.0);
  memcpy(&p2->p[3], p, sizeof(vec2)); // p2 3
  p2->color = e->color;

  point(p, e, 2/3.0);
  memcpy(&p3->p[0], p, sizeof(vec2)); // p3 0

  mix(a, e->p[1], e->p[2], 2/3.0);
  mix(b, e->p[2], e->p[3], 2/3.0);
  mix(p, a, b, 2/3.0);
  memcpy(&p3->p[1], p, sizeof(vec2)); // p3 1

  if (e->p[2] == e->p[3]) {
    memcpy(&p3->p[2], e->p[3], sizeof(vec2)); // ? p3 2
  } else {
    mix(p, e->p[2], e->p[3], 2/3.0);
    memcpy(&p3->p[2], p, sizeof(vec2)); // ? p3 2
  }

  memcpy(&p3->p[3], e->p[3], sizeof(vec2)); // p3 3
}

void edge_split(edge_segment_t *e, edge_segment_t *p1, edge_segment_t *p2, edge_segment_t *p3)
{
  switch (e->type) {
    case STBTT_vline: {
      linear_split(e, p1, p2, p3);
      break;
    }
    case STBTT_vcurve: {
      quadratic_split(e, p1, p2, p3);
      break;
    }
    case STBTT_vcubic: {
      cubic_split(e, p1, p2, p3);
      break;
    }
  }
}

double shoelace(const vec2 a, const vec2 b)
{
  return (b[0]-a[0])*(a[1]+b[1]);
}

float* ex_msdf_glyph(stbtt_fontinfo *font, uint32_t c, size_t w, size_t h, ex_metrics_t *metrics)
{
  float *bitmap = malloc(sizeof(float)*3*w*h);
  memset(bitmap, 0.0f, sizeof(float)*3*w*h);

  // Funit to pixel scale
  float scale = stbtt_ScaleForMappingEmToPixels(font, h);

  // get left offset and advance
  int left_bearing, advance;
  stbtt_GetGlyphHMetrics(font, stbtt_FindGlyphIndex(font, c), &advance, &left_bearing);
  left_bearing *= scale;

  // get glyph bounding box (scaled later)
  int ix0, iy0, ix1, iy1;
  stbtt_GetGlyphBox(font, stbtt_FindGlyphIndex(font, c), &ix0, &iy0, &ix1, &iy1);

  // calculate offset for centering glyph on bitmap
  int translate_x = (w / 2) - ((ix1 - ix0) * scale) / 2 - left_bearing;
  int translate_y = (h / 2) - ((iy1 - iy0) * scale) / 2 - iy0 * scale;

  // set the glyph metrics
  // (pre-scale them)
  if (metrics) {
      metrics->left_bearing = left_bearing;
      metrics->advance = advance * scale;
      metrics->ix0 = ix0 * scale;
      metrics->ix1 = ix1 * scale;
      metrics->iy0 = iy0 * scale;
      metrics->iy1 = iy1 * scale;
  }

  stbtt_vertex *verts;
  int num_verts = stbtt_GetGlyphShape(font, stbtt_FindGlyphIndex(font, c), &verts);
  
  // figure out how many contours exist
  int contour_count = 0;
  for (int i=0; i<num_verts; i++) {
    if (verts[i].type == STBTT_vmove)
      contour_count++;
  }

  if (contour_count == 0) {
    free(bitmap);
    return NULL;
  }

  // determin what vertices belong to what contours
  typedef struct {
    size_t start,end;
  } indices_t;
  indices_t *contours = malloc(sizeof(indices_t) * contour_count);
  int j = 0;
  for (int i=0; i<=num_verts; i++) {
    if (verts[i].type == STBTT_vmove) {
      if (i > 0) {
        contours[j].end = i;
        j++;
      }

      contours[j].start = i;
    } else if (i >= num_verts) {
      contours[j].end = i;
    }
  }

  typedef struct {
    signed_distance_t min_distance;
    edge_segment_t *near_edge;
    double near_param;
  } edge_point_t;

  typedef struct {
    edge_segment_t *edges;
    size_t edge_count;
  } contour_t;
  
  // process verts into series of contour-specific edge lists
  vec2 initial = {0, 0}; // fix this?
  contour_t *contour_data = malloc(sizeof(contour_t) * contour_count);
  for (int i=0; i<contour_count; i++) {
    size_t count = contours[i].end - contours[i].start;
    contour_data[i].edges = malloc(sizeof(edge_segment_t) * count);
    contour_data[i].edge_count = 0;

    size_t k = 0;
    for (int j=contours[i].start; j<contours[i].end; j++) {

      edge_segment_t *e = &contour_data[i].edges[k];
      stbtt_vertex *v   = &verts[j];
      e->type  = v->type;
      e->color = WHITE;

      switch (v->type) {
        case STBTT_vmove: {
          vec2 p = {v->x/64.0, v->y/64.0};
          memcpy(&initial, p, sizeof(vec2));
          break;
        }

        case STBTT_vline: {
          vec2 p = {v->x/64.0, v->y/64.0};
          memcpy(&e->p[0], initial, sizeof(vec2));
          memcpy(&e->p[1], p, sizeof(vec2));
          memcpy(&initial, p, sizeof(vec2));
          contour_data[i].edge_count++;
          k++;
          break;
        }

        case STBTT_vcurve: {
          vec2 p = {v->x/64.0, v->y/64.0};
          vec2 c = {v->cx/64.0, v->cy/64.0};
          memcpy(&e->p[0], initial, sizeof(vec2));
          memcpy(&e->p[1], c, sizeof(vec2));
          memcpy(&e->p[2], p, sizeof(vec2));
          
          if ((e->p[0][0] == e->p[1][0] && e->p[0][1] == e->p[1][1]) ||
              (e->p[1][0] == e->p[2][0] && e->p[1][1] == e->p[2][1])) { 
            e->p[1][0] = 0.5*(e->p[0][0]+e->p[2][0]);
            e->p[1][1] = 0.5*(e->p[0][1]+e->p[2][1]);
          }

          memcpy(&initial, p, sizeof(vec2));
          contour_data[i].edge_count++;
          k++;
          break;
        }

        case STBTT_vcubic: {
          vec2 p = {v->x/64.0, v->y/64.0};
          vec2 c = {v->cx/64.0, v->cy/64.0};
          vec2 c1 = {v->cx1/64.0, v->cy1/64.0};
          memcpy(&e->p[0], initial, sizeof(vec2));
          memcpy(&e->p[1], c, sizeof(vec2));
          memcpy(&e->p[2], c1, sizeof(vec2));
          memcpy(&e->p[3], p, sizeof(vec2));
          memcpy(&initial, p, sizeof(vec2));
          contour_data[i].edge_count++;
          k++;
          break;
        }
      } 
    }
  }

  // calculate edge-colors
  unsigned long long seed = 0;
  double anglethreshold = 3.0;
  double crossthreshold = sin(anglethreshold);
  size_t corner_count = 0;
  for (int i=0; i<contour_count; ++i)
    for (int j=0; j<contour_data[i].edge_count; ++j)
      corner_count++;

  int *corners = malloc(sizeof(int) * corner_count);
  int corner_index = 0;
  for (int i=0; i<contour_count; ++i) {
      
    if (contour_data[i].edge_count > 0) {
      vec2 prev_dir, dir;
      direction(prev_dir, &contour_data[i].edges[contour_data[i].edge_count-1], 1);

      int index = 0;
      for (int j=0; j<contour_data[i].edge_count; ++j, ++index) {
        edge_segment_t *e = &contour_data[i].edges[j];
        direction(dir, e, 0);
        vec2_norm(dir, dir);
        vec2_norm(prev_dir, prev_dir);
        if (is_corner(prev_dir, dir, crossthreshold))
          corners[corner_index++] = index;
        direction(prev_dir, e, 1);
      }
    }

    if (corner_index == 0) {
      for (int j=0; j<contour_data[i].edge_count; ++j)
        contour_data[i].edges[j].color = WHITE;
    } else if (corner_index == 1) {
      edge_color_t colors[3] = {WHITE, WHITE};
      switch_color(&colors[0], &seed, BLACK);
      colors[2] = colors[0];
      switch_color(&colors[2], &seed, BLACK);

      int corner = corners[0];
      if (contour_data[i].edge_count >= 3) {
        int m = contour_data[i].edge_count;
        for (int j=0; j<m; ++j)
          contour_data[i].edges[(corner+j)%m].color = (colors+1)[(int)(3+2.875*i/(m-1)-1.4375+.5)-3];
      } else if (contour_data[i].edge_count >= 1) {
        edge_segment_t *parts[7] = {NULL};
        edge_split(&contour_data[i].edges[0], parts[0+3*corner], parts[1+3*corner], parts[2+3*corner]);
        if (contour_data[i].edge_count >= 2) {
          edge_split(&contour_data[i].edges[1], parts[3-3*corner], parts[4-3*corner], parts[5-3*corner]);
          parts[0]->color = parts[1]->color = colors[0];
          parts[2]->color = parts[3]->color = colors[1];
          parts[4]->color = parts[5]->color = colors[2];
        } else {
          parts[0]->color = colors[0];
          parts[1]->color = colors[1];
          parts[2]->color = colors[2];
        }
        free(contour_data[i].edges);
        contour_data[i].edges = malloc(sizeof(edge_segment_t) * 7);
        contour_data[i].edge_count = 0;
        int index = 0;
        for (int j = 0; parts[j]; ++j) {
          memcpy(&contour_data[i].edges[index++], &parts[j], sizeof(edge_segment_t));
          contour_data[i].edge_count++;
        }
      }
    } else {
      int spline = 0;
      int start = corners[0];
      int m = contour_data[i].edge_count;
      edge_color_t color = WHITE;
      switch_color(&color, &seed, BLACK);
      edge_color_t initial_color = color;
      for (int j=0; j<m; ++j) {
        int index = (start+j)%m;
        if (spline+1 < corner_count && corners[spline+1] == index) {
          ++spline;

          edge_color_t s = (edge_color_t)((spline == corner_count-1)*initial_color);
          switch_color(&color, &seed, s);
        }
        contour_data[i].edges[index].color = color;
      }
    }
  }

  free(corners);

  // normalize shape
  for (int i=0; i<contour_count; i++) {
    if (contour_data[i].edge_count == 1) {
      edge_segment_t *parts[3] = {NULL};
      edge_split(&contour_data[i].edges[0], parts[0], parts[1], parts[2]);
      free(contour_data[i].edges);
      contour_data[i].edges = malloc(sizeof(edge_segment_t) * 3);
      contour_data[i].edge_count = 3;
      for (int j=0; j<3; j++)
        memcpy(&contour_data[i].edges[j], &parts[j], sizeof(edge_segment_t));
    }
  }

  // calculate windings
  int *windings = malloc(sizeof(int) * contour_count);
  for (int i=0; i<contour_count; i++) {
    size_t edge_count = contour_data[i].edge_count;
    if (edge_count == 0) {
      windings[i] = 0;
      continue;
    }

    double total = 0;

    if (edge_count == 1) {
      vec2 a,b,c;
      point(a, &contour_data[i].edges[0], 0);
      point(b, &contour_data[i].edges[0], 1/3.0);
      point(c, &contour_data[i].edges[0], 2/3.0);
      total += shoelace(a, b);
      total += shoelace(b, c);
      total += shoelace(c, a);
    } else if (edge_count == 2) {
      vec2 a,b,c,d;
      point(a, &contour_data[i].edges[0], 0);
      point(b, &contour_data[i].edges[0], 0.5);
      point(c, &contour_data[i].edges[1], 0);
      point(d, &contour_data[i].edges[1], 0.5);
      total += shoelace(a, b);
      total += shoelace(b, c);
      total += shoelace(c, d);
      total += shoelace(d, a);
    } else {
      vec2 prev;
      point(prev, &contour_data[i].edges[edge_count-1], 0);
      for (int j=0; j<edge_count; j++) {
        vec2 cur;
        point(cur, &contour_data[i].edges[j], 0);
        total += shoelace(prev, cur);
        memcpy(prev, cur, sizeof(vec2));
      }
    }

    windings[i] = ((0 < total)-(total < 0));
  }

  typedef struct {
    double r, g, b;
    double med;
  } multi_distance_t;

  multi_distance_t *contour_sd;
  contour_sd = malloc(sizeof(multi_distance_t) * contour_count);

  // offset scale for base metrics
  // scale *= 64.0;

  for (int y=0; y<h; ++y) {
    int row = iy0 > iy1 ? y : h-y-1;
    for (int x=0; x<w; ++x) {
      vec2 p = {(x+.5-translate_x)/(scale*64.0), (y+.5-translate_y)/(scale*64.0)};

      edge_point_t sr, sg, sb;
      sr.near_edge = sg.near_edge = sb.near_edge = NULL;
      sr.near_param = sg.near_param = sb.near_param = 0;
      sr.min_distance.dist = sg.min_distance.dist = sb.min_distance.dist = INF;
      sr.min_distance.d = sg.min_distance.d = sb.min_distance.d = 1;
      double d = fabs(INF);
      double neg_dist = -INF;
      double pos_dist = INF;
      int winding = 0;

      for (int j=0; j<contour_count; ++j) {
        edge_point_t r, g, b;
        r.near_edge = g.near_edge = b.near_edge = NULL;
        r.near_param = g.near_param = b.near_param = 0;
        r.min_distance.dist = g.min_distance.dist = b.min_distance.dist = INF;
        r.min_distance.d = g.min_distance.d = b.min_distance.d = 1;

        for (int k=0; k<contour_data[j].edge_count; ++k) {
          edge_segment_t *e = &contour_data[j].edges[k];
          double param;
          signed_distance_t distance;
          distance.dist = INF;
          distance.d = 1;

          // calculate signed distance
          switch (e->type) {
            case STBTT_vline: {
              distance = linear_dist(e, p, &param);
              break;
            }
            case STBTT_vcurve: {
              distance = quadratic_dist(e, p, &param);
              break;
            }
            case STBTT_vcubic: {
              distance = cubic_dist(e, p, &param);
              break;
            }
          }

          if (e->color&RED && signed_compare(distance, r.min_distance)) {
            r.min_distance = distance;
            r.near_edge    = e;
            r.near_param   = param;
          }
          if (e->color&GREEN && signed_compare(distance, g.min_distance)) {
            g.min_distance = distance;
            g.near_edge    = e;
            g.near_param   = param;
          }
          if (e->color&BLUE && signed_compare(distance, b.min_distance)) {
            b.min_distance = distance;
            b.near_edge    = e;
            b.near_param   = param;
          }
        }

        if (signed_compare(r.min_distance, sr.min_distance))
          sr = r;
        if (signed_compare(g.min_distance, sg.min_distance))
          sg = g;
        if (signed_compare(b.min_distance, sb.min_distance))
          sb = b;

        double med_min_dist = fabs(median(r.min_distance.dist, g.min_distance.dist, b.min_distance.dist));

        if (med_min_dist < d) {
          d = med_min_dist;
          winding = -windings[j];
        }

        if (r.near_edge)
          dist_to_pseudo(&r.min_distance, p, r.near_param, r.near_edge);
        if (g.near_edge)
          dist_to_pseudo(&g.min_distance, p, g.near_param, g.near_edge);
        if (b.near_edge)
          dist_to_pseudo(&b.min_distance, p, b.near_param, b.near_edge);

        med_min_dist = median(r.min_distance.dist, g.min_distance.dist, b.min_distance.dist);
        contour_sd[j].r = r.min_distance.dist;
        contour_sd[j].g = g.min_distance.dist;
        contour_sd[j].b = b.min_distance.dist;
        contour_sd[j].med = med_min_dist;

        if (windings[j] > 0 && med_min_dist >= 0 && fabs(med_min_dist) < fabs(pos_dist))
          pos_dist = med_min_dist;
        if (windings[j] < 0 && med_min_dist <= 0 && fabs(med_min_dist) < fabs(neg_dist))
          neg_dist = med_min_dist;
      }

      if (sr.near_edge)
        dist_to_pseudo(&sr.min_distance, p, sr.near_param, sr.near_edge);
      if (sg.near_edge)
        dist_to_pseudo(&sg.min_distance, p, sg.near_param, sg.near_edge);
      if (sb.near_edge)
        dist_to_pseudo(&sb.min_distance, p, sb.near_param, sb.near_edge);
    
      multi_distance_t msd;
      msd.r = msd.g = msd.b = msd.med = INF;
      if (pos_dist >= 0 && fabs(pos_dist) <= fabs(neg_dist)) {
        msd.med = INF;
        winding = 1;
        for (int i=0; i<contour_count; ++i)
          if (windings[i] > 0 && contour_sd[i].med > msd.med && fabs(contour_sd[i].med) < fabs(neg_dist))
            msd = contour_sd[i];
      } else if (neg_dist <= 0 && fabs(neg_dist) <= fabs(pos_dist)) {
        msd.med = -INF;
        winding = -1;
        for (int i=0; i<contour_count; ++i)
          if (windings[i] < 0 && contour_sd[i].med < msd.med && fabs(contour_sd[i].med) < fabs(pos_dist))
            msd = contour_sd[i];
      }

      for (int i=0; i<contour_count; ++i)
        if (windings[i] != winding && fabs(contour_sd[i].med) < fabs(msd.med))
          msd = contour_sd[i];

      if (median(sr.min_distance.dist, sg.min_distance.dist, sb.min_distance.dist) == msd.med) {
        msd.r = sr.min_distance.dist;
        msd.g = sg.min_distance.dist;
        msd.b = sb.min_distance.dist;
      }

      size_t index = 3*((row*w)+x);
      bitmap[index] = (float)msd.r/RANGE+.5;   // r
      bitmap[index+1] = (float)msd.g/RANGE+.5; // g
      bitmap[index+2] = (float)msd.b/RANGE+.5; // b
    }
  }
  for (int i=0; i<contour_count; i++)
    free(contour_data[i].edges);
  free(contour_data);
  free(contour_sd);
  free(contours);
  free(windings);
  free(verts);

  // msdf error correction
  typedef struct {
    int x, y;
  } clashes_t;
  clashes_t *clashes = malloc(sizeof(clashes_t) * w * h);
  size_t cindex = 0;

  double tx = EDGE_THRESHOLD/(scale*RANGE);
  double ty = EDGE_THRESHOLD/(scale*RANGE);
  for (int y=0; y<h; y++) {
    for (int x=0; x<w; x++) {
      if ((x > 0  && pixel_clash(P(x, y, w, bitmap), P(x-1, y, w, bitmap), tx))
      || (x < w-1 && pixel_clash(P(x, y, w, bitmap), P(x+1, y, w, bitmap), tx))
      || (y > 0   && pixel_clash(P(x, y, w, bitmap), P(x, y-1, w, bitmap), ty))
      || (y < h-1 && pixel_clash(P(x, y, w, bitmap), P(x, y+1, w, bitmap), ty))
        ) {
          clashes[cindex].x   = x;
          clashes[cindex++].y = y;
      }
    }
  }

  for (int i=0; i<cindex; i++) {
    size_t index = 3*((clashes[i].y*w)+clashes[i].x);
    float med = median(bitmap[index], bitmap[index+1], bitmap[index+2]);
    bitmap[index+0] = med;
    bitmap[index+1] = med;
    bitmap[index+2] = med;
  }
  free(clashes);

  return bitmap;
}