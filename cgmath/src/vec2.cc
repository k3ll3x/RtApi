#include "vec2.h"
#include <math.h>

cgmath::vec2::vec2() : x(0.0f), y(0.0f)
{
}

cgmath::vec2::vec2(float x, float y) : x(x), y(y)
{
}

float & cgmath::vec2::operator[](int i)
{
	float * xy[] = {&x,&y};
	return * xy[i];
}

const float & cgmath::vec2::operator[](int i) const
{
	const float* xy[] = { &x,&y };
	return *xy[i];
}

cgmath::vec2 & cgmath::vec2::operator*=(float s)
{
	x = x * s;
	y = y * s;
	return *this;
}

cgmath::vec2 & cgmath::vec2::operator/=(float s)
{
	x = x / s;
	y = y / s;
	return *this;
}

cgmath::vec2 & cgmath::vec2::operator+=(const vec2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

cgmath::vec2 & cgmath::vec2::operator-=(const vec2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

bool cgmath::vec2::operator==(const vec2 & v) const
{
	if (x == v.x && y == v.y) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

float cgmath::vec2::magnitude() const
{
	return sqrt((x*x) + (y*y));
}

float cgmath::vec2::magnitude(const vec2 & v)
{
	return sqrt((v.x*v.x) + (v.y*v.y));
}

void cgmath::vec2::normalize()
{
	float m = this->magnitude();
	x = x / m;
	y = y / m;
}

cgmath::vec2 cgmath::vec2::normalize(const vec2 & v)
{
	float m = v.magnitude();
	return vec2(v.x/m,v.y/m);
}

float cgmath::vec2::dot(const vec2 & a, const vec2 & b)
{
	return (a.x * b.x) + (a.y * b.y);
}

std::ostream & cgmath::operator<<(std::ostream & os, const vec2 & v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}
