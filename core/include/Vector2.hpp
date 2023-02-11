#pragma once

template<typename T>
class Vector2
{
public:
	T x, y;

	static const Vector2 ZERO;
	static const Vector2 ONE;

	// constructors

	constexpr Vector2(): x(static_cast<T>(0)), y(static_cast<T>(0)) {}
	explicit constexpr Vector2(T s): x(s), y(s) {}
	constexpr Vector2(T nx, T ny): x(nx), y(ny) {}
	template<typename A>
	explicit constexpr Vector2(A a): x(static_cast<T>(a)), y(static_cast<T>(a))
	{
	}

	template<typename A, typename B>
	constexpr Vector2(A a, B b): x(static_cast<T>(a)), y(static_cast<T>(b))
	{
	}

	explicit constexpr Vector2(T* p): x(p[0]), y(p[1]) {}

	template<typename A>
	explicit constexpr Vector2(A* pa)
	        : x(static_cast<T>(pa[0])), y(static_cast<T>(pa[1]))
	{
	}

	template<typename A>
	constexpr Vector2(const Vector2<A>& v): Vector2(v.x, v.y)
	{
	}

	constexpr Vector2(const Vector2<T>&) = default;
	constexpr Vector2(Vector2<T>&&)      = default;

	template<typename A>
	Vector2<T>& operator=(const Vector2<A>& v)
	{
		x = static_cast<T>(v.x);
		y = static_cast<T>(v.y);
		return *this;
	}

	Vector2<T>& operator=(const Vector2<T>&) = default;
	Vector2<T>& operator=(Vector2<T>&&)      = default;

	// unary operations

	T& operator[](int i) { return i == 0 ? x : y; }
	T  operator[](int i) const { return i == 0 ? x : y; }

	constexpr Vector2<T> operator+() const { return *this; }
	constexpr Vector2<T> operator-() const { return Vector2<T>(-x, -y); }

	Vector2<T>& operator++()
	{
		++x;
		++y;
		return *this;
	}

	Vector2<T>& operator--()
	{
		--x;
		--y;
		return *this;
	}

	Vector2<T> operator++(int)
	{
		Vector2<T> v = *this;
		++x;
		++y;
		return v;
	}

	Vector2<T> operator--(int)
	{
		Vector2<T> v = *this;
		--x;
		--y;
		return v;
	}

	constexpr explicit operator bool() const
	{
		return (x != static_cast<T>(0)) && (y != static_cast<T>(0));
	}

	// logical operations

	constexpr bool operator==(Vector2<T> r) const { return x == r.x && y == r.y; }
	constexpr bool operator!=(Vector2<T> r) const { return !operator==(r); }
	constexpr bool operator>=(Vector2<T> r) const { return x >= r.x && y >= r.y; }
	constexpr bool operator<=(Vector2<T> r) const { return x <= r.x && y <= r.y; }
	constexpr bool operator<(Vector2<T> r) const { return !operator>=(r); }
	constexpr bool operator>(Vector2<T> r) const { return !operator<=(r); }

	// vector-vector operations

	constexpr Vector2<T> operator+(Vector2<T> r) const
	{
		return Vector2<T>(x + r.x, y + r.y);
	}

	constexpr Vector2<T> operator-(Vector2<T> r) const
	{
		return Vector2<T>(x - r.x, y - r.y);
	}

	constexpr Vector2<T> operator*(Vector2<T> r) const
	{
		return Vector2<T>(x * r.x, y * r.y);
	}

	constexpr Vector2<T> operator/(Vector2<T> r) const
	{
		return Vector2<T>(x / r.x, y / r.y);
	}

	Vector2<T>& operator+=(Vector2<T> r) const
	{
		x += r.x;
		y += r.y;
		return *this;
	}

	Vector2<T>& operator-=(Vector2<T> r) const
	{
		x -= r.x;
		y -= r.y;
		return *this;
	}

	Vector2<T>& operator*=(Vector2<T> r) const
	{
		x *= r.x;
		y *= r.y;
		return *this;
	}

	Vector2<T>& operator/=(Vector2<T> r) const
	{
		x /= r.x;
		y /= r.y;
		return *this;
	}

	// vector-scaler operations

	constexpr Vector2<T> operator+(T r) const { return Vector2<T>(x + r, y + r); }
	constexpr Vector2<T> operator-(T r) const { return Vector2<T>(x - r, y - r); }
	constexpr Vector2<T> operator*(T r) const { return Vector2<T>(x * r, y * r); }
	constexpr Vector2<T> operator/(T r) const { return Vector2<T>(x / r, y / r); }

	Vector2<T>& operator+=(T r) const
	{
		x += r;
		y += r;
		return *this;
	}

	Vector2<T>& operator-=(T r) const
	{
		x -= r;
		y -= r;
		return *this;
	}

	Vector2<T>& operator*=(T r) const
	{
		x *= r;
		y *= r;
		return *this;
	}

	Vector2<T>& operator/=(T r) const
	{
		x /= r;
		y /= r;
		return *this;
	}

	// functions

	T Length() const { return static_cast<T>(std::sqrt(x * x + y * y)); }

	constexpr T LengthSqrd() const { return x * x + y * y; }

	void Normalize()
	{
		T t = Length();
		x /= t;
		y /= t;
	}

	Vector2<T> Normalize() const
	{
		Vector2<T> v = *this;
		return v.Normalize();
	}

	bool IsNormalized() const { return Length() == static_cast<T>(1); }

	void Zero() { y = x = static_cast<T>(0); }
	void One() { y = x = static_cast<T>(1); }
};

// scalar-vector operations

template<typename T>
Vector2<T> operator+(T l, Vector2<T> r)
{
	return Vector2<T>(l + r.x, l + r.y);
}

template<typename T>
Vector2<T> operator-(T l, Vector2<T> r)
{
	return Vector2<T>(l - r.x, l - r.y);
}

template<typename T>
Vector2<T> operator*(T l, Vector2<T> r)
{
	return Vector2<T>(l * r.x, l * r.y);
}

template<typename T>
Vector2<T> operator/(T l, Vector2<T> r)
{
	return Vector2<T>(l / r.x, l / r.y);
}

template<typename T>
const Vector2<T> Vector2<T>::ZERO(0);

template<typename T>
const Vector2<T> Vector2<T>::ONE(1);

using Vec2   = Vector2<float>;
using Vec2i  = Vector2<int>;
using Vec2ui = Vector2<unsigned int>;