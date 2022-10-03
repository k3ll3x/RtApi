#pragma once
#include <iostream>
#include "vec3.h"

#define SIZEM3 3

namespace cgmath {
	class mat3 {
	private:
		float n[SIZEM3][SIZEM3];
	public:
		//constructors
		mat3();
		mat3(float diagonal);
		mat3(const vec3& a, const vec3& b, const vec3& c);

		//operators
		vec3& operator[](int column);
		const vec3& operator[](int column) const;
		bool operator==(const mat3& m) const;

		//methods
		static float determinant(const mat3& m);
		static mat3 inverse(const mat3& m);
		static mat3 transpose(const mat3& m);
		
		//operators
		friend std::ostream& operator<<(std::ostream& os, const mat3& m);
	};

	//inline operators
	inline vec3 operator*(const mat3& m, const vec3& v) {
		vec3 x = m[0] * v[0];
		vec3 y = m[1] * v[1];
		vec3 z = m[2] * v[2];
		return vec3(x[0] + y[0] + z[0], x[1] + y[1] + z[1], x[2] + y[2] + z[2]);
	}
	inline mat3 operator*(const mat3& m1, const mat3& m2) {
		mat3 n;
		for (int i = 0; i < SIZEM3;i++) {
			vec3 v1 = m1[0] * m2[i][0];
			vec3 v2 = m1[1] * m2[i][1];
			vec3 v3 = m1[2] * m2[i][2];
			vec3 v4 = v1 + v2 + v3;
			n[i] = v4;
		}
		return n;
	}
}
