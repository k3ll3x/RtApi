#include "vec4.h"
#include <math.h>

cgmath::vec4::vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

cgmath::vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
{
}

cgmath::vec4::vec4(cgmath::vec3 &v, float w)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

float & cgmath::vec4::operator[](int i)
{
	float * xyzw[] = { &x,&y,&z,&w };
	return *xyzw[i];
}

const float & cgmath::vec4::operator[](int i) const
{
	const float* xyzw[] = { &x,&y,&z,&w };
	return *xyzw[i];
}

cgmath::vec4 & cgmath::vec4::operator*=(float s)
{
	x = x * s;
	y = y * s;
	z = z * s;
	w = w * s;
	return *this;
}

cgmath::vec4 & cgmath::vec4::operator/=(float s)
{
	x = x / s;
	y = y / s;
	z = z / s;
	w = w / s;
	return *this;
}

cgmath::vec4 & cgmath::vec4::operator+=(const vec4 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

cgmath::vec4 & cgmath::vec4::operator-=(const vec4 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

bool cgmath::vec4::operator==(const vec4 & v) const
{
	if (x == v.x && y == v.y && z == v.z && w == v.w) {
		return true;
	}
	else {
		return false;
	}
	return false;
}

float cgmath::vec4::magnitude() const
{
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

void cgmath::vec4::normalize()
{
	x = x / this->magnitude();
	y = y / this->magnitude();
	z = z / this->magnitude();
	w = w / this->magnitude();
}

float cgmath::vec4::magnitude(const vec4 & v)
{
	return sqrt((v.x*v.x) + (v.y*v.y) + (v.z*v.z) + (v.w*v.w));
}

cgmath::vec4 cgmath::vec4::normalize(const vec4 & v)
{
	return vec4(v.x / v.magnitude(), v.y / v.magnitude(), v.z / v.magnitude(), v.w / v.magnitude());
}

float cgmath::vec4::dot(const vec4 & a, const vec4 & b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

std::ostream & cgmath::operator<<(std::ostream & os, const vec4 & v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}
