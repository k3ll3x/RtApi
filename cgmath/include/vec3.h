#pragma once
#include <iostream>
#include <vec2.h>

namespace cgmath {
	class vec3 {
	public:
		float x;
		float y;
		float z;

		//constructors
		vec3();
		vec3(float x, float y, float z);
		vec3(cgmath::vec2 &v,float z);

		//operators
		float& operator[](int i);
		const float& operator[](int i) const;
		vec3& operator*=(float s);
		vec3& operator/=(float s);
		vec3& operator+=(const vec3& v);
		vec3& operator-=(const vec3& v);
		bool operator==(const vec3& v) const;

		//methods
		float magnitude() const;
		void normalize();
		static float magnitude(const vec3& v);
		static vec3 normalize(const vec3& v);
		static float dot(const vec3& a, const vec3& b);
		static vec3 cross(const vec3& a, const vec3& b);

		//operators
		friend std::ostream& operator<<(std::ostream& os, const vec3& v);
	};

	//inline operators
	inline vec3 operator*(const vec3& v, float s) {
		return vec3(v.x*s, v.y*s, v.z*s);
	}
	inline vec3 operator*(float s, const vec3& v) {
		return vec3(v.x*s, v.y*s, v.z*s);
	}
	inline vec3 operator/(const vec3& v, float s) {
		return vec3(v.x / s, v.y / s,v.z / s);
	}
	inline vec3 operator+(const vec3& a, const vec3& b) {
		return vec3(a.x + b.x, a.y + b.y,a.z + b.z);
	}
	inline vec3 operator-(const vec3& a, const vec3& b) {
		return vec3(a.x - b.x, a.y - b.y,a.z - b.z);
	}
	inline vec3 operator-(const vec3& v) {
		return vec3(-v.x, -v.y, -v.z);
	}
}
