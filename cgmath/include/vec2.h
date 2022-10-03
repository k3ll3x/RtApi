#pragma once
#include <iostream>

namespace cgmath {
	class vec2 {
	public:
		float x;
		float y;

		//constructors
		vec2();
		vec2(float x,float y);

		//operators
		float& operator[](int i);
		const float& operator[](int i) const;
		vec2& operator*=(float s);
		vec2& operator/=(float s);
		vec2& operator+=(const vec2& v);
		vec2& operator-=(const vec2& v);
		bool operator==(const vec2& v) const;

		//methods
		float magnitude() const;
		void normalize();
		static float magnitude(const vec2& v);
		static vec2 normalize(const vec2& v);
		static float dot(const vec2& a,const vec2& b);

		//operators
		friend std::ostream& operator<<(std::ostream& os, const vec2& v);
	};

	//inline operators
	inline vec2 operator*(const vec2& v, float s) {
		return vec2(v.x*s, v.y*s);
	}
	inline vec2 operator*(float s, const vec2& v) {
		return vec2(v.x*s, v.y*s);
	}
	inline vec2 operator/(const vec2& v, float s) {
		return vec2(v.x / s, v.y / s);
	}
	inline vec2 operator+(const vec2& a, const vec2& b) {
		return vec2(a.x + b.x, a.y + b.y);
	}
	inline vec2 operator-(const vec2& a, const vec2& b) {
		return vec2(a.x - b.x, a.y - b.y);
	}
	inline vec2 operator-(const vec2& v) {
		return vec2(-v.x, -v.y);
	}
}
