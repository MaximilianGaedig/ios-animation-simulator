#ifndef COLOR_H
#define COLOR_H

#include "types.h"
#include <iostream>

class color {
public:
  decimal_t e[4];

  color() : e{0, 0, 0, 0} {}
  color(decimal_t e0, decimal_t e1, decimal_t e2, decimal_t e3)
      : e{e0, e1, e2, e3} {}

  decimal_t r() const { return e[0]; }
  decimal_t g() const { return e[1]; }
  decimal_t b() const { return e[2]; }
  decimal_t a() const { return e[3]; }

  color operator-() const { return color(-e[0], -e[1], -e[2], -e[3]); }

  decimal_t operator[](int i) const { return e[i]; }
  decimal_t &operator[](int i) { return e[i]; }

  color &operator+=(const color &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    e[3] += v.e[3];
    return *this;
  }

  color &operator*=(decimal_t t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    e[3] *= t;
    return *this;
  }

  color &operator/=(decimal_t t) { return *this *= 1 / t; }
};

using point3 = color;

inline std::ostream &operator<<(std::ostream &out, const color &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline color operator+(const color &u, const color &v) {
  return color(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2],
               u.e[3] + v.e[3]);
}

inline color operator-(const color &u, const color &v) {
  return color(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2],
               u.e[3] + v.e[3]);
}

// multiplies each coordinate with the same coordinate from the other vector
inline color operator*(const color &u, const color &v) {
  return color(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2],
               u.e[3] * v.e[3]);
}

// multiplies each coordinate with the float
inline color operator*(decimal_t t, const color &v) {
  return color(t * v.e[0], t * v.e[1], t * v.e[2], t * v.e[3]);
}

inline color operator*(const color &v, decimal_t t) { return t * v; }

inline color operator/(color v, decimal_t t) { return (1 / t) * v; }

// Write the translated [0,255] value of each color component.
inline void write_color(std::ostream &out, color pixel_color) {
  // .999 is used because of floating point rounding weirdness
  out << static_cast<int>(255.999 * pixel_color.r()) << ' '
      << static_cast<int>(255.999 * pixel_color.g()) << ' '
      << static_cast<int>(255.999 * pixel_color.b()) << ' ' << '\n';
}

#endif
