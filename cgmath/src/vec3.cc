#include "vec3.h"
#include <math.h>

cgmath::vec3::vec3() : x(0.0f), y(0.0f), z(0.0f)
{
}

cgmath::vec3::vec3(float x, float y, float z) : x(x), y(y), z(z)
{
}

cgmath::vec3::vec3(vec2 &v, float z)
{
	this->x = v.x;
	this->y = v.y;
	this->z = z;
}

float& cgmath::vec3::operator[](int i)
{
	float * xyz[] = { &x,&y,&z };
	return *xyz[i];
}

const float& cgmath::vec3::operator[](int i) const
{
	const float* xyz[] = { &x,&y,&z };
	return *xyz[i];
}

cgmath::vec3 & cgmath::vec3::operator*=(float s)
{
	x = x * s;
	y = y * s;
	z = z * s;
	return *this;
}

cgmath::vec3 & cgmath::vec3::operator/=(float s)
{
	x = x / s;
	y = y / s;
	z = z / s;
	return *this;
}

cgmath::vec3 & cgmath::vec3::operator+=(const vec3 & v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

cgmath::vec3 & cgmath::vec3::operator-=(const vec3 & v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

bool cgmath::vec3::operator==(const vec3 & v) const
{
	if (x == v.x && y == v.y && z == v.z) {
		return true;
	}
	return false;
}

float cgmath::vec3::magnitude() const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

void cgmath::vec3::normalize()
{
	float m = this->magnitude();
	x = x / m;
	y = y / m;
	z = z / m;
}

float cgmath::vec3::magnitude(const vec3 & v)
{
	return sqrt((v.x*v.x) + (v.y*v.y) + (v.z*v.z));
}

cgmath::vec3 cgmath::vec3::normalize(const vec3 & v)
{
	return vec3(v.x / v.magnitude(), v.y / v.magnitude(), v.z / v.magnitude());
}

float cgmath::vec3::dot(const vec3 & a, const vec3 & b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

cgmath::vec3 cgmath::vec3::cross(const vec3 & a, const vec3 & b)
{
	return vec3((a.y * b.z)-(a.z * b.y),-((a.x * b.z) - (a.z * b.x)),(a.x * b.y)-(a.y * b.x));
}

std::ostream& cgmath::operator<<(std::ostream & os, const vec3 & v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}
