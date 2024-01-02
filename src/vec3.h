#ifndef VEC3_H
#define VEC3_H

#include "types.h"
#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
public:
  decimal_t e[3];

  vec3() : e{0, 0, 0} {}
  vec3(decimal_t e0, decimal_t e1, decimal_t e2) : e{e0, e1, e2} {}

  decimal_t x() const { return e[0]; }
  decimal_t y() const { return e[1]; }
  decimal_t z() const { return e[2]; }

  vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

  decimal_t operator[](int i) const { return e[i]; }
  decimal_t &operator[](int i) { return e[i]; }

  vec3 &operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
  }

  vec3 &operator*=(decimal_t t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
  }

  vec3 &operator/=(decimal_t t) { return *this *= 1 / t; }

  decimal_t length() const { return sqrt(length_squared()); }

  decimal_t length_squared() const {
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }
};

using point3 = vec3;

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// multiplies each coordinate with the same coordinate from the other vector
inline vec3 operator*(const vec3 &u, const vec3 &v) {
  return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// multiplies each coordinate with the float
inline vec3 operator*(decimal_t t, const vec3 &v) {
  return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, decimal_t t) { return t * v; }

inline vec3 operator/(vec3 v, decimal_t t) { return (1 / t) * v; }

inline decimal_t dot(const vec3 &u, const vec3 &v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
  return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
              u.e[2] * v.e[0] - u.e[0] * v.e[2],
              u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

#endif
