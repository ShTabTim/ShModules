#ifndef SH_MATH_LIN_GEM_2D_HPP
#define SH_MATH_LIN_GEM_2D_HPP

#include "../basic_math_includes.hpp"

template<typename T> class vec2 {
public:
    T x = 0, y = 0;
    vec2() {}
    template<typename O> vec2(O _x) : x(_x), y(_x) {}
    template<typename Ox, typename Oy> vec2(Ox _x, Oy _y) : x(_x), y(_y) {}
    template<typename O> vec2(vec2<O> v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}
    template<typename O> inline auto operator()(vec2<O> v) const -> decltype(x * v.x) { return x * v.x + y * v.y; }
};

template<typename T> using mat2x2 = vec2<vec2<T>>;

template<typename T> class sbf2 {
public:
    T x = 0, xy = 0, y = 0;
    sbf2() {}
    template<typename Ox, typename Oxy, typename Oy> sbf2(Ox _x, Oxy _xy, Oy _y) : x(_x), xy(_xy), y(_y) {}
    mat2x2<T> to_mat() const { return mat2x2<T>{ vec2<T>{x, xy}, vec2<T>{xy, y} }; }
    template<typename O> sbf2(sbf2<O> g) : x(static_cast<T>(g.x)), xy(static_cast<T>(g.xy)), y(static_cast<T>(g.y)) {}
    template<typename O> inline auto operator()(vec2<O> v) const -> vec2<decltype(x * v.x)> { return vec2<decltype(x * v.x)>(x * v.x + xy * v.y, xy * v.x + y * v.y); }
    template<typename V1, typename V2> inline auto operator()(vec2<V1> v1, vec2<V2> v2) const -> decltype(x * v1.x * v2.x) { return x * v1.x * v2.x + xy * (v1.x * v2.y + v1.y * v2.x) + y * v1.y * v2.y; }
};

template<typename T> inline vec2<T> operator+(const vec2<T>& v0, const vec2<T>& v1) { return vec2<T>{ v0.x + v1.x, v0.y + v1.y }; }
template<typename T> inline vec2<T> operator-(const vec2<T>& v0, const vec2<T>& v1) { return vec2<T>{ v0.x - v1.x, v0.y - v1.y }; }
template<typename T> inline sbf2<T> operator+(const sbf2<T>& g0, const sbf2<T>& g1) { return sbf2<T>{ g0.x + g1.x, g0.xy + g1.xy, g0.y + g1.y }; }
template<typename T> inline sbf2<T> operator-(const sbf2<T>& g0, const sbf2<T>& g1) { return sbf2<T>{ g0.x - g1.x, g0.xy - g1.xy, g0.y - g1.y }; }

template<typename T> inline vec2<T> operator-(const vec2<T>& v) { return vec2<T>{ -v.x, -v.y }; }
template<typename T, typename O> inline auto operator*(const vec2<T>& v, const O& s) -> vec2<decltype(v.x * s)> { return vec2<decltype(v.x * s)>{ v.x * s, v.y * s }; }
template<typename T, typename O> inline auto operator*(const O& s, const vec2<T>& v) -> vec2<decltype(s * v.x)> { return vec2<decltype(s * v.x)>{ s * v.x, s * v.y }; }
template<typename T, typename O> inline auto operator/(const vec2<T>& v, const O& s) -> vec2<decltype(v.x / s)> { return vec2<decltype(v.x / s)>{ v.x / s, v.y / s }; }

template<typename T> inline sbf2<T> operator-(const sbf2<T>& g) { return sbf2<T>{ -g.x, -g.xy, -g.y }; }
template<typename T, typename O> inline auto operator*(const sbf2<T>& g, const O& s) -> sbf2<decltype(g.x * s)> { return sbf2<decltype(g.x * s)>{ g.x * s, g.xy * s, g.y * s }; }
template<typename T, typename O> inline auto operator*(const O& s, const sbf2<T>& g) -> sbf2<decltype(s * g.x)> { return sbf2<decltype(s * g.x)>{ s * g.x, s * g.xy, s * g.y }; }
template<typename T, typename O> inline auto operator/(const sbf2<T>& g, const O& s) -> sbf2<decltype(g.x / s)> { return sbf2<decltype(g.x / s)>{ g.x / s, g.xy / s, g.y / s }; }

template<typename V1, typename V2> inline auto operator*(const mat2x2<V1>& m1, const mat2x2<V2>& m2) -> mat2x2<decltype(m1.x.x * m2.x.x)> { return vec2<decltype(m1.x.x * m2.x.x)>{ m1(m2.x), m1(m2.y) }; }

template<typename T> inline auto norm(const vec2<T>& v) { return std::sqrt(v.operator()(v)); }
template<typename T, typename O> inline auto norm(const vec2<T>& v, const sbf2<O>& g) { return std::sqrt(std::abs(g(v,v))); }
template<typename T> inline auto normalized(const vec2<T>& v) { return v / norm(v); }
template<typename T, typename O> inline auto normalized(const vec2<T>& v, const sbf2<O>& g) { return v / norm(v, g); }

template<typename T> inline T cross(const vec2<T>& v1, const vec2<T>& v2) { return v1.x * v2.y - v1.y * v2.x; }
template<typename T> inline T det(const mat2x2<T>& m) { return m.x.x * m.y.y - m.x.y * m.y.x; }
template<typename T> inline T det(const sbf2<T>& g) { return g.x * g.y - g.xy * g.xy; }
template<typename T> inline bool is_positive(const sbf2<T>& g) { return (g.x > 0) && (det(g) > 0); }
template<typename T> inline bool is_volumed(const sbf2<T>& g) { return det(g) > 0; }
template<typename T> inline bool is_negative(const sbf2<T>& g) { return (g.x < 0) && (det(g) > 0); }
template<typename T> inline bool is_degenerate(const sbf2<T>& g) { return det(g) == 0; }
template<typename T> inline bool is_degenerate(const mat2x2<T>& m) { return det(m) == 0; }
template<typename T> inline mat2x2<T> adj(const mat2x2<T>& m) { return mat2x2<T>{ vec2<T>{ m.y.y, -m.x.y }, vec2<T>{ -m.y.x, m.x.x } }; }
template<typename T> inline mat2x2<T> inv(const mat2x2<T>& m) { return adj(m) / det(m); }
template<typename T> inline sbf2<T> adj(const sbf2<T>& g) { return sbf2<T>{ g.y, -g.xy, g.x }; }
template<typename T> inline sbf2<T> inv(const sbf2<T>& g) { return adj(g) / det(g); }
template<typename T> inline mat2x2<T> transpose(const mat2x2<T>& m) { return mat2x2<T>{ vec2<T>{ m.x.x, m.y.x }, vec2<T>{ m.x.y, m.y.y } }; }

#endif//SH_MATH_LIN_GEM_2D_HPP