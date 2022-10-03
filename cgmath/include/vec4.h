#pragma once
#include <iostream>
#include "vec3.h"

namespace cgmath {
	class vec4 {
	public:
		float x;
		float y;
		float z;
		float w;

		//constructors
		vec4();
		vec4(float x, float y, float z, float w);
		vec4(cgmath::vec3 &v, float w);

		//operators
		float& operator[](int i);
		const float& operator[](int i) const;
		vec4& operator*=(float s);
		vec4& operator/=(float s);
		vec4& operator+=(const vec4& v);
		vec4& operator-=(const vec4& v);
		bool operator==(const vec4& v) const;

		//methods
		float magnitude() const;
		void normalize();
		static float magnitude(const vec4& v);
		static vec4 normalize(const vec4& v);
		static float dot(const vec4& a, const vec4& b);

		//operators
		friend std::ostream& operator<<(std::ostream& os, const vec4& v);
	};

	//inline operators
	inline vec4 operator*(const vec4& v, float s) {
		return vec4(v.x*s, v.y*s, v.z*s, v.w*s);
	}
	inline vec4 operator*(float s, const vec4& v) {
		return vec4(v.x*s, v.y*s, v.z*s, v.w*s);
	}
	inline vec4 operator/(const vec4& v, float s) {
		return vec4(v.x / s, v.y / s, v.z / s, v.w / s);
	}
	inline vec4 operator+(const vec4& a, const vec4& b) {
		return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
	inline vec4 operator-(const vec4& a, const vec4& b) {
		return vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}
	inline vec4 operator-(const vec4& v) {
		return vec4(-v.x, -v.y, -v.z, -v.w);
	}
}
