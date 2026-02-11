#include "vlinal.c"

struct square
  {
  struct vec4 p[4];
  };

struct cube
  {
  struct square p[6];
  };

struct square square(float size)
  {
  return (struct square)
    {
    .p[0].f = {0, 0, 0, 1},
    .p[1].f = {0+size, 0, 0, 1},
    .p[2].f = {0, 0+size, 0, 1},
    .p[3].f = {0+size, 0+size, 0, 1},
    };
  }

#define square3d(x, y, z, lx, ly, lz) \
0+x,   0+y,  0+z,\
lx+x,  0+y,  0+z,\
0+x,  ly+y, lz+z,\
lx+x, ly+y, lz+z,

#define squareindices(offset)\
offset+0, offset+1, offset+2,\
offset+2, offset+1, offset+3,

#define cube(x, y, z, l)\
square3d(x,   y,   z, l, l, 0)\
square3d(x,   y, l+z, l, l, 0)\
square3d(x,   y,   z, l, 0, l)\
square3d(x, l+y,   z, l, 0, l)\
square3d(x,   y,   z, 0, l, l)\
square3d(x+l, y,   z, 0, l, l)\

#define cubeindices(offset)\
squareindices(offset)\
squareindices(offset+4)\
squareindices(offset+4+4)\
squareindices(offset+4+4+4)\
squareindices(offset+4+4+4+4)\
squareindices(offset+4+4+4+4+4)\

