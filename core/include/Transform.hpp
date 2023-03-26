#pragma once

#include <Common.hpp>
#include <MathFunctions.hpp>
#include <Vector2.hpp>

struct Transform
{
private:
	float m[6];

public:
	constexpr Transform(const Transform&)            = default;
	constexpr Transform(Transform&&)                 = default;
	constexpr Transform& operator=(const Transform&) = default;
	constexpr Transform& operator=(Transform&&)      = default;

	constexpr Transform(): m {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f} {}

	explicit constexpr Transform(float s): m {s, 0.f, 0.f, 0.f, s, 0.f} {}

	template<typename A>
	explicit constexpr Transform(A a): Transform(static_cast<float>(a))
	{
	}

	constexpr Transform(float m1, float m2, float m3, float m4, float m5, float m6)
	        : m {m1, m2, m3, m4, m5, m6}
	{
	}

	template<typename A, typename B, typename C, typename D, typename E, typename F>
	constexpr Transform(A m1, B m2, C m3, D m4, E m5, F m6)
	        : m {static_cast<float>(m1), static_cast<float>(m2),
	             static_cast<float>(m3), static_cast<float>(m4),
	             static_cast<float>(m5), static_cast<float>(m6)}
	{
	}

	Transform(float left, float right, float bottom, float top): Transform()
	{
		Project(left, right, bottom, top);
	}

	Vec2       operator*(Vec2 v) const;
	Transform  operator*(const Transform& r) const;
	Transform& operator*=(const Transform& r)
	{
		*this = *this * r;
		return *this;
	};

	friend std::ostream& operator<<(std::ostream& os, const Transform& transform);

	constexpr const float* GetPtr() const { return m; }

	void Reset();

	Transform&        Translate(float x, float y);
	inline Transform& Translate(Vec2 v) { return Translate(v.x, v.y); }
	inline Transform& Translate(float s) { return Translate(s, s); }

	Transform&        Scale(float x, float y);
	inline Transform& Scale(Vec2 v) { return Scale(v.x, v.y); }
	inline Transform& Scale(float s) { return Scale(s, s); }

	Transform&        Shear(float x, float y);
	inline Transform& Shear(Vec2 v) { return Shear(v.x, v.y); }
	inline Transform& Shear(float f) { return Shear(f, f); }

	// angle in degrees
	Transform&        Rotate(float angle);
	inline Transform& RotateRadians(float angle)
	{
		return Rotate((float)Rad2Deg(angle));
	}

	Transform& Project(float left, float right, float bottom, float top);

	Transform& Invert();
};

std::ostream& operator<<(std::ostream& os, const Transform& transform);
