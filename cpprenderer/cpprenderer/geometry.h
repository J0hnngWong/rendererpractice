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
};

template <class t>
struct Vector3 {
    union {
        struct {t x, y, z;};
        struct {t ivertex, iuv, inorm;};
        t raw[3];
    };
};

typedef Vector3<float> VectorFloat3;

#endif /* geometry_h */
