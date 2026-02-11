struct vec4 { float f[4]; };
struct mat4 { struct vec4 v[4]; };

#define ihat4 (struct vec4) { .f = {1, 0, 0, 0} }
#define jhat4 (struct vec4) { .f = {0, 1, 0, 0} }
#define khat4 (struct vec4) { .f = {0, 0, 1, 0} }
#define lhat4 (struct vec4) { .f = {0, 0, 0, 1} }

#define lincon4(v, m) trans4(v, m)

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

struct vec4 trans4(struct vec4 v, struct mat4 m)
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
      trans4(a.v[0], b),
      trans4(a.v[1], b),
      trans4(a.v[2], b),
      trans4(a.v[3], b),
    } };
  }






