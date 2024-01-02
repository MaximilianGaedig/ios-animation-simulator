#ifndef VEC3_H
#define VEC3_H

#include "types.h"
#include <cmath>
#include <iostream>

using std::sqrt;

class vec2 {
public:
  decimal_t e[2];

  vec2() : e{0, 0} {}
  vec2(decimal_t e0, decimal_t e1) : e{e0, e1} {}

  decimal_t x() const { return e[0]; }
  decimal_t y() const { return e[1]; }

  vec2 operator-() const { return vec2(-e[0], -e[1]); }

  decimal_t operator[](int i) const { return e[i]; }
  decimal_t &operator[](int i) { return e[i]; }

  vec2 &operator+=(const vec2 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    return *this;
  }

  vec2 &operator*=(decimal_t t) {
    e[0] *= t;
    e[1] *= t;
    return *this;
  }

  vec2 &operator/=(decimal_t t) { return *this *= 1 / t; }

  decimal_t length() const { return sqrt(length_squared()); }

  decimal_t length_squared() const { return e[0] * e[0] + e[1] * e[1]; }
};

using point2 = vec2;

inline std::ostream &operator<<(std::ostream &out, const vec2 &v) {
  return out << v.e[0] << ' ' << v.e[1];
}

inline vec2 operator+(const vec2 &u, const vec2 &v) {
  return vec2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
}

inline vec2 operator-(const vec2 &u, const vec2 &v) {
  return vec2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
}

// multiplies each coordinate with the same coordinate from the other vector
inline vec2 operator*(const vec2 &u, const vec2 &v) {
  return vec2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
}

// multiplies each coordinate with the float
inline vec2 operator*(decimal_t t, const vec2 &v) {
  return vec2(t * v.e[0], t * v.e[1]);
}

inline vec2 operator*(const vec2 &v, decimal_t t) { return t * v; }

inline vec2 operator/(vec2 v, decimal_t t) { return (1 / t) * v; }

inline decimal_t dot(const vec2 &u, const vec2 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1];
}

inline vec2 unit_vector(vec2 v) { return v / v.length(); }

#endif
