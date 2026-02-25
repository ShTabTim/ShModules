#ifndef SH_MATH_3D_QUAT_HPP
#define SH_MATH_3D_QUAT_HPP

#include "basic_math_includes.hpp"
#include "vec3.hpp"
#include "mat3.hpp"

template<typename T>
class quat {
public:
    vec3<T> v = {0, 0, 0};
    T r = 0;

    inline quat() = default;
    template<typename O> explicit quat(const vec3<O>& o) { r = 0; v = o; }
    template<typename O> explicit quat(const quat<O>& o) { r = o.r; v = o.v; }
    template<typename O> explicit quat(const O& o) { r = o; v = {0, 0, 0}; }
    template<typename O> explicit quat(const vec3<O>& oo, const O& o) { v = oo; r = o; }
    template<typename O> explicit quat(const O& xo, const O& yo, const O& zo, const O& ro) { r = ro; v = {xo, yo, zo}; }


    template <typename O> inline bool operator==(const vec3<O>& o) const { return r == o.r && v == o.v; }
    template <typename O> inline bool operator!=(const vec3<O>& o) const { return r != o.r || v != o.v; }

    inline quat<T> operator-() const { return quat<T>(-v, -r); }
    inline quat<T>& operator+=(const quat<T>& o) { r+=o.r; v+=o.v; return *this; }
    inline quat<T>& operator-=(const quat<T>& o) { r-=o.r; v-=o.v; return *this; }
    template <typename O> inline quat<T> operator+(const quat<O>& o) const { return quat<T>(*this)+=o; }
    template <typename O> inline quat<T> operator-(const quat<O>& o) const { return quat<T>(*this)-=o; }
    template <typename O> inline quat<T> operator*(const O& o) { return quat<T>(v * o, r * o); }
    template <typename O> inline quat<T> operator*(const quat<O>& o) { return quat<T>(r * o.v + o.r * v + cross(v, o.v), r*o.r - dot(v, o.v)); }
    template <typename O> inline quat<T>& operator/=(const O& o) { r/=o; v/=o; return *this; }
    template <typename O> inline quat<T> operator/(const O& o) { return quat<T>(*this)/=o; }
    template <typename O> inline quat<T> operator/(const quat<O>& o) { return conj(*this)/abs2(*this); }
};

template<typename T> inline quat<T> conj(const quat<T>& q) { return quat<T>(-q.v, q.r); }
template<typename T> inline T abs2(const quat<T>& q) { return q.r*q.r + abs2(q.v); }

#ifdef  USE_STANDART_MATH
template<typename T> inline float abs(const quat<T>& q) { return sqrtf(abs2(q)); }

template<typename T> inline quat<T> rot(const vec3<T>& normal) { T s = abs(normal)/2; return {cos(s), normal*(2*sin(s)/s)}; }
template<typename T> inline quat<float> rot(const vec3<T>& normal, float alpha) { alpha *= 0.5f; return quat<float>(normal*sinf(alpha), cosf(alpha)); }
#endif//USE_STANDART_MATH

template<typename T> inline vec3<T> transform(const vec3<T>& a, const quat<T>& q) { return a*(q.r*q.r - abs2(q.v)) + (2*q.r)*cross(q.v, a) + (2*dot(q.v, a))*a; }//q*a*conj(q)

template<typename T> inline mat3<T> rot(const quat<T>& q) {
    return mat3<T>(
            q.r*q.r+q.v.x*q.v.x-q.v.y*q.v.y-q.v.z*q.v.z, 2*(q.v.x*q.v.y-q.v.z*q.r), 2*(q.v.x*q.v.z+q.v.y*q.r),
            2*(q.v.x*q.v.y+q.v.z*q.r), q.r*q.r-q.v.x*q.v.x+q.v.y*q.v.y-q.v.z*q.v.z, 2*(q.v.y*q.v.z-q.v.x*q.r),
            2*(q.v.x*q.v.z-q.v.y*q.r), 2*(q.v.y*q.v.z+q.v.x*q.r), q.r*q.r-q.v.x*q.v.x-q.v.y*q.v.y+q.v.z*q.v.z
    );
}

template<typename T> inline quat<T> inv(const quat<T>& q) { return conj(q)/abs2(q); }

#ifdef  USE_STANDART_MATH
template<typename T> inline quat<float> normalize(const quat<T>& q) { return conj(q)/abs(q); }
#endif//USE_STANDART_MATH

#endif//SH_MATH_3D_QUAT_HPP