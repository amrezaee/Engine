#include <Transform.hpp>

vec2 Transform::operator*(vec2 v) const
{
	return {m[0] * v.x + m[1] * v.y + m[2], m[3] * v.x + m[4] * v.y + m[5]};
}

Transform Transform::operator*(const Transform& r) const
{
	return {m[0] * r.m[0] + m[1] * r.m[3],
	        m[0] * r.m[1] + m[1] * r.m[4],
	        m[0] * r.m[2] + m[1] * r.m[5] + m[2],
	        m[3] * r.m[0] + m[4] * r.m[3],
	        m[3] * r.m[1] + m[4] * r.m[4],
	        m[3] * r.m[2] + m[4] * r.m[5] + m[5]};
}

std::ostream& operator<<(std::ostream& os, const Transform& transform)
{
	os << transform.m[0] << ' ' << transform.m[1] << ' ' << transform.m[2] << '\n';
	os << transform.m[3] << ' ' << transform.m[4] << ' ' << transform.m[5] << '\n';
	return os;
}

void Transform::Reset()
{
	m[0] = 1.0f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 1.0f;
	m[5] = 0.0f;
}

Transform& Transform::Translate(float x, float y)
{
	m[2] = m[0] * x + m[1] * y + m[2];
	m[5] = m[3] * x + m[4] * y + m[5];
	return *this;
}

Transform& Transform::Scale(float x, float y)
{
	m[0] *= x;
	m[1] *= y;
	m[3] *= x;
	m[4] *= y;
	return *this;
}

Transform& Transform::Shear(float x, float y)
{
	float a = m[0];
	float d = m[3];
	m[0]    += m[1] * y;
	m[1]    += a * x;
	m[3]    += m[4] * y;
	m[4]    += d * x;
	return *this;
}

Transform& Transform::Rotate(float angle)
{
	auto  rad = (float)Deg2Rad(angle);
	float c   = std::cos(rad);
	float s   = std::sin(rad);
	float a   = m[0];
	float b   = m[1];
	float d   = m[3];
	float e   = m[4];
	m[0]      = a * c + b * s;
	m[1]      = b * c - a * s;
	m[3]      = d * c + e * s;
	m[4]      = e * c - d * s;
	return *this;
}

Transform& Transform::Project(float left, float right, float bottom, float top)
{
	float rl = (right - left);
	float tb = (top - bottom);
	float a  = 2.0f / rl;
	float b  = 2.0f / tb;
	float c  = (-right - left) / rl;
	float d  = (-top - bottom) / tb;

	m[5] += (c * m[3]) + (d * m[4]);
	m[4] *= b;
	m[3] *= a;
	m[2] += (c * m[0]) + (d * m[1]);
	m[1] *= b;
	m[0] *= a;
	return *this;
}

Transform& Transform::Invert()
{
	float det = m[0] * m[4] - m[1] * m[3];
	float m0  = m[0];
	float m2  = m[2];

	m[2] = -(m[2] * m[4] - m[1] * m[5]) / det;
	m[5] = -(m[0] * m[5] - m2 * m[3]) / det;
	m[0] = +m[4] / det;
	m[1] = -m[1] / det;
	m[3] = -m[3] / det;
	m[4] = +m0 / det;
	return *this;
}
