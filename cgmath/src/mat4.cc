#include "mat4.h"

cgmath::mat4::mat4()
{
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			this->n[j][i] = 0;
		}
	}
}

cgmath::mat4::mat4(float diagonal)
{
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			if (i == j) {
				this->n[j][i] = diagonal;
			}
			else {
				this->n[j][i] = 0;
			}
		}
	}
}

cgmath::mat4::mat4(const vec4 & a, const vec4 & b, const vec4 & c, const vec4 & d)
{
	for (int i = 0; i < SIZEM4; i++) {
		this->n[0][i] = a[i];
	}
	for (int i = 0; i < SIZEM4; i++) {
		this->n[1][i] = b[i];
	}
	for (int i = 0; i < SIZEM4; i++) {
		this->n[2][i] = c[i];
	}
	for (int i = 0; i < SIZEM4; i++) {
		this->n[3][i] = d[i];
	}
}

cgmath::vec4 & cgmath::mat4::operator[](int column)
{
	vec4 * a = reinterpret_cast<vec4*>(this->n[column]);
	return *a;
}

const cgmath::vec4 & cgmath::mat4::operator[](int column) const
{
	const vec4 * a = reinterpret_cast<const vec4*>(this->n[column]);
	return *a;
}

bool cgmath::mat4::operator==(const mat4 & m) const
{
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			if (this->n[i][j] == m.n[i][j]) {
				return true;
			}
			else {
				return false;
			}
		}
	}
	return false;
}


float cgmath::mat4::determinant(const mat4 & m)
{
	vec3 v1(m[1].y, m[1].z, m[1].w);
	vec3 v2(m[2].y, m[2].z, m[2].w);
	vec3 v3(m[3].y, m[3].z, m[3].w);
	mat3 m1(v1,v2,v3);
	v1 = vec3(m[0].y, m[0].z, m[0].w);
	v2 = vec3(m[2].y, m[2].z, m[2].w);
	v3 = vec3(m[3].y, m[3].z, m[3].w);
	mat3 m2(v1,v2,v3);
	v1 = vec3(m[0].y, m[0].z, m[0].w);
	v2 = vec3(m[1].y, m[1].z, m[1].w);
	v3 = vec3(m[3].y, m[3].z, m[3].w);
	mat3 m3(v1,v2,v3);
	v1 = vec3(m[0].y, m[0].z, m[0].w);
	v2 = vec3(m[1].y, m[1].z, m[1].w);
	v3 = vec3(m[2].y, m[2].z, m[2].w);
	mat3 m4(v1,v2,v3);

	float tmp[8] = {m[0][0],m1.determinant(m1),m[1][0],m2.determinant(m2),m[2][0],m3.determinant(m3),m[3][0],m4.determinant(m4)};
	float d = tmp[0] * tmp[1] - tmp[2] * tmp[3] + tmp[4] * tmp[5] - tmp[6] * tmp[7];
	return d;
}

cgmath::mat4 cgmath::mat4::inverse(const mat4 & m)
{
	float det = m.determinant(m);
	return m.adj(m) * (1 / det);
}

cgmath::mat4 cgmath::mat4::transpose(const mat4 & m)
{
	vec4 v1(m[0][0], m[1][0], m[2][0], m[3][0]);
	vec4 v2(m[0][1], m[1][1], m[2][1], m[3][1]);
	vec4 v3(m[0][2], m[1][2], m[2][2], m[3][2]);
	vec4 v4(m[0][3], m[1][3], m[2][3], m[3][3]);
	return mat4(v1, v2, v3, v4);
}

cgmath::mat3 cgmath::mat4::matrixij(const mat4 & m, int c, int r)
{
	mat3 m3;
	float arr[9];
	int counter = 0;
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			if (i != r && j != c) {
				arr[counter] = m[j][i];
				counter++;
			}
		}
	}
	counter = 0;
	for (int i = 0; i < SIZEM3; i++) {
		for (int j = 0; j < SIZEM3; j++) {
			m3[j][i] = arr[counter];
			counter++;
		}
	}
	return m3;
}

cgmath::mat4 cgmath::mat4::adj(const mat4 & m)
{
	mat4 adj;
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			mat3 aij = m.matrixij(m,j,i);
			float aijdet = aij.determinant(aij);
			adj[i][j] = pow(-1.0f, i + j) * aijdet;
		}
	}
	return adj;
}

std::ostream & cgmath::operator<<(std::ostream & os, const mat4 & m)
{
	for (int i = 0; i < SIZEM4; i++) {
		for (int j = 0; j < SIZEM4; j++) {
			os << m.n[j][i];
			if (j != SIZEM4 - 1) {
				os << " ";
			}
		}
		if (i != SIZEM4 - 1) {
			os << "\n";
		}
	}
	return os;
}
