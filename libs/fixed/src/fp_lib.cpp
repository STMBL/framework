#include <fp_lib.hpp>

int32_t cos(int32_t x){ // only fract bits < 0.5
  const int32_t p = 1000.5 * (1 << 16);
  const int32_t m = 0.5 * (1 << 16);
  x += p;
  x &= 0x0000ffff;
  x -= m;
  x >>= 3;

  const int32_t a = 1 * (1 << 13);
  const int32_t b = -19.5556 * (1 << 13);
  const int32_t c = 60.4444 * (1 << 13);
  const int32_t d = -56.8889 * (1 << 13);

  int32_t xx = (x * x) >> 13; // f.bits
  int32_t dxx = (d * xx) >> 13; // f.24
  int32_t cdxx = c + dxx; // f.24
  int32_t xxcdxx = (xx * cdxx) >> 13; // f.24
  int32_t bxxcdxx = b + xxcdxx; // f.24
  int32_t xxbxxcdxx = (xx * bxxcdxx) >> 13; // f.24
  int32_t axxbxxcdxx = a + xxbxxcdxx; // f.24
  axxbxxcdxx <<= 3;

  return(axxbxxcdxx);
}

inline int32_t sin(int32_t x){
  const int32_t p = 0.25 * (1 << 16);
  return(cos(x - p));
}


// TODO
// %
// sqrt
// atan2
// +-*/ int, float
// int, float constructor
// math const, pi, sqrt(2), sqrt(3), ...
// <>==!= int, fixed