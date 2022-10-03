#pragma once
#include <iostream>
#include "vec4.h"
#include "mat3.h"
#include <math.h> //pow

#define SIZEM4 4

namespace cgmath {
	class mat4 {
	private:
		float n[SIZEM4][SIZEM4];
	public:
		//constructors
		mat4();
		mat4(float diagonal);
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d);

		//operators
		vec4& operator[](int column);
		const vec4& operator[](int column) const;
		bool operator==(const mat4& m) const;

		//methods
		static float determinant(const mat4& m);
		static mat4 inverse(const mat4& m);
		static mat4 transpose(const mat4& m);
		static mat3 matrixij(const mat4& m, int c, int r);
		static mat4 adj(const mat4& m);

		//operators
		friend std::ostream& operator<<(std::ostream& os, const mat4& m);
	};

	//inline operators
	inline vec4 operator*(const mat4& m, const vec4& v) {
		vec4 x = m[0] * v[0];
		vec4 y = m[1] * v[1];
		vec4 z = m[2] * v[2];
		vec4 w = m[3] * v[3];
		return vec4(x[0] + y[0] + z[0] + w[0], x[1] + y[1] + z[1] + w[1], x[2] + y[2] + z[2] + w[2],x[3] + y[3] + z[3] + w[3]);
	}
	inline mat4 operator*(const mat4& m1, const mat4& m2) {
		mat4 n;
		for (int i = 0; i < SIZEM4; i++) {
			vec4 v1 = m1[0] * m2[i][0];
			vec4 v2 = m1[1] * m2[i][1];
			vec4 v3 = m1[2] * m2[i][2];
			vec4 v4 = m1[3] * m2[i][3];
			vec4 v5 = v1 + v2 + v3 + v4;
			n[i] = v5;
		}
		return n;
	}
}
