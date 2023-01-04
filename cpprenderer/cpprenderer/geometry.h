//
//  geometry.h
//  cpprenderer
//
//  Created by john on 2022/12/30.
//

#ifndef geometry_h
#define geometry_h

#include <cmath>

template <class t> struct Vector2 {
    union {
        struct {t u, v;};
        struct {t x, y;};
        t raw[2];
    };
    Vector2() : u(0), v(0) {}
    Vector2(t _u, t _v) : u(_u), v(_v) {}
    inline Vector2<t> operator +(const Vector2<t> &V) const { return Vector2<t>(u+V.u, v+V.v); }
    inline Vector2<t> operator -(const Vector2<t> &V) const { return Vector2<t>(u-V.u, v-V.v); }
    inline Vector2<t> operator *(float f)             const { return Vector2<t>(u*f, v*f); }

};

template <class t>
struct Vector3 {
    union {
        struct {t x, y, z;};
        struct {t ivertex, iuv, inorm;};
        t raw[3];
    };
    Vector3() : x(0), y(0), z(0) {}
    Vector3(t _x, t _y, t _z): x(_x), y(_y), z(_z) {}

    float length() const { return std::sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }
};

typedef Vector3<float> VectorFloat3;

#endif /* geometry_h */
