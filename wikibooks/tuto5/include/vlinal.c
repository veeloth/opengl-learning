#pragma once

#include <math.h>
#include <stddef.h>
struct vec4 { float f[4]; };
struct mat4 { struct vec4 v[4]; };
struct fa16 { float f[16]; };//float array, use this to send data to OpenGL, generate them from a mat4 using mat4_fa16

#define ihat4 (struct vec4) { .f = {1, 0, 0, 0} }
#define jhat4 (struct vec4) { .f = {0, 1, 0, 0} }
#define khat4 (struct vec4) { .f = {0, 0, 1, 0} }
#define lhat4 (struct vec4) { .f = {0, 0, 0, 1} }
#define iden4 (struct mat4) { .v = {ihat4, jhat4, khat4, lhat4 } }

//#define lincon4(v, m) trans4(v, m)

struct fa16 mat4_fa16(struct mat4 m)
  {
  return (struct fa16)
    {.f = {
      m.v[0].f[0], m.v[1].f[0], m.v[2].f[0], m.v[3].f[0], 
      m.v[0].f[1], m.v[1].f[1], m.v[2].f[1], m.v[3].f[1], 
      m.v[0].f[2], m.v[1].f[2], m.v[2].f[2], m.v[3].f[2], 
      m.v[0].f[3], m.v[1].f[3], m.v[2].f[3], m.v[3].f[3], 
    } };
  }

//float* fa16_gl(struct fa16* m) { return m->f; }

struct vec4 scale4(float scalar, struct vec4 src)
  {
  return (struct vec4)
    {.f = {
      src.f[0] *= scalar,
      src.f[1] *= scalar,
      src.f[2] *= scalar,
      src.f[3] *= scalar,
    } };
  }

struct vec4 sum4(struct vec4 a, struct vec4 b)
  {
  return (struct vec4)
    {.f = {
      a.f[0] += b.f[0],
      a.f[1] += b.f[1],
      a.f[2] += b.f[2],
      a.f[3] += b.f[3],
    } };
  }

struct vec4 sum4_4(struct vec4 a, struct vec4 b, struct vec4 c, struct vec4 d)
  {
  return (struct vec4)
    {.f = {
      a.f[0] += b.f[0] + c.f[0] + d.f[0],
      a.f[1] += b.f[1] + c.f[1] + d.f[1],
      a.f[2] += b.f[2] + c.f[2] + d.f[2],
      a.f[3] += b.f[3] + c.f[3] + d.f[3],
    } };
  }

struct vec4 lincon4(struct vec4 v, struct mat4 m)
  {
  return sum4_4
    (
    scale4(v.f[0], m.v[0]),
    scale4(v.f[1], m.v[1]),
    scale4(v.f[2], m.v[2]),
    scale4(v.f[3], m.v[3])
    );
  }

struct mat4 mul4(struct mat4 a, struct mat4 b)
  {
  return (struct mat4)
    {.v = {
      lincon4(a.v[0], b),
      lincon4(a.v[1], b),
      lincon4(a.v[2], b),
      lincon4(a.v[3], b),
    } };
  }


struct mat4 trans4(float x, float y, float z)
  {
  return (struct mat4)
    {.v = {
      { 1, 0, 0, x, },
      { 0, 1, 0, y, },
      { 0, 0, 1, z, },
      { 0, 0, 0, 1, },
    } };
  }

struct mat4 rot4(float angle, size_t a, size_t b)//sizes a and b indicate the index of the axis vectors that will be affected
  {
  struct mat4 ret = iden4;
  ret.v[a].f[a] = cosf(angle);
  ret.v[a].f[b] = sinf(angle);
  ret.v[b].f[a] = -sinf(angle);
  ret.v[b].f[b] = cosf(angle);
  return ret;
  }

struct mat4 pers4()
  {
  const float r = 0.5;
  const float t = 0.5;
  const float n = 1;
  const float f = 5;
  return (struct mat4)
    { .v = {
      { n/r, 0, 0, 0, },
      { 0, n/t, 0, 0, },
      { 0, 0, (-f - n)/(f-n), (2*f*n)/(n-f), },
      { 0, 0, -1, 0, },
    } };
  }






