#ifndef VECTOR
#define VECTOR

#include <stdbool.h>
#include <math.h>

typedef struct Vec2i {
  int x, y;
} Vec2i;

static inline Vec2i AddVec2i(Vec2i a, Vec2i b)      { return (Vec2i){a.x + b.x, a.y + b.y}; }
static inline Vec2i SubtractVec2i(Vec2i a, Vec2i b) { return (Vec2i){a.x - b.x, a.y - b.y}; }
static inline Vec2i MultVec2i(Vec2i v, int num)     { return (Vec2i){v.x * num, v.y * num}; }
static inline bool isEqualVec2i(Vec2i a, Vec2i b)   { return a.x == b.x && a.y == b.y;      }
static inline float DistanceVec2i(Vec2i a, Vec2i b) { return sqrtf(powf(b.x-a.x, 2) + powf(b.y-a.y, 2));}

#endif