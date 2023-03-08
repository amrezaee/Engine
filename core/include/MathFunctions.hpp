#pragma once

#include <Common.hpp>

constexpr real CPI      = 3.1415926535;
constexpr real CPI_180  = CPI / 180.0;
constexpr real C180_PI  = 180.0 / CPI;
constexpr real CPI_SQRD = CPI * CPI;
constexpr real CPI_HALF = CPI * 0.5;

constexpr real Deg2Rad(real deg)
{
	return deg * CPI_180;
}

constexpr real Rad2Deg(real rad)
{
	return rad * C180_PI;
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
