#include "math.inc"

float LinearizeDepth(sampler2D dBuffer, vec2 uv)
{
  float n = 0.5;		// camera z near
  float f = 1000.0;		// camera z far
  float z = texture(dBuffer, uv).x;
  float C = 2.0 / log2(f + 1);
  z = (pow(C*f+1,z)-1)/C;
  return z/f;
}