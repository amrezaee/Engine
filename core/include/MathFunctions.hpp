#pragma once

#include <Common.hpp>

constexpr float CPI      = 3.14159f;
constexpr float CPI_180  = CPI / 180.0f;
constexpr float C180_PI  = 180.0f / CPI;
constexpr float CPI_SQRD = CPI * CPI;
constexpr float CPI_HALF = CPI * 0.5f;

constexpr float Deg2Rad(const float deg)
{
	return deg * CPI_180;
}

constexpr float Rad2Deg(const float rad)
{
	return rad * C180_PI;
}

template<typename T>
constexpr const T& Clamp(const T& v, const T& l, const T& h)
{
	return (v < l) ? l : ((h < v) ? h : v);
}

template<typename T>
constexpr const T& Max(const T& a, const T& b)
{
	return (a < b) ? b : a;
}

template<typename T>
constexpr const T& Min(const T& a, const T& b)
{
	return (a < b) ? a : b;
}

// clang-format off
template<typename T>
constexpr typename std::enable_if_t<std::is_floating_point_v<T>, T>
Lerp(T a, T b, T t)
{
	return (static_cast<T>(1) - t) * a + t * b;
}

template<typename T>
constexpr typename std::enable_if_t<std::is_integral_v<T>, T>
Lerp(T a, T b, T t)
{
	return a + t * (b - a);
}

template<typename T>
constexpr typename std::enable_if_t<std::is_signed_v<T>, int>
Sign(T v)
{
	return (static_cast<T>(0) < v) - (v < static_cast<T>(0));
}
// clang-format on