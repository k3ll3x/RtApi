#include "mat3.h"

cgmath::mat3::mat3()
{
	for (int i = 0; i < SIZEM3; i++) {
		for (int j = 0; j < SIZEM3; j++) {
			this->n[j][i] = 0;
		}
	}
}

cgmath::mat3::mat3(float diagonal)
{
	for (int i = 0; i < SIZEM3; i++) {
		for (int j = 0; j < SIZEM3; j++) {
			if (i == j) {
				this->n[j][i] = diagonal;
			}
			else {
				this->n[j][i] = 0;
			}
		}
	}
}

cgmath::mat3::mat3(const vec3& a, const vec3& b, const vec3& c)
{
	for (int i = 0; i < SIZEM3; i++) {
		this->n[0][i] = a[i];
	}
	for (int i = 0; i < SIZEM3; i++) {
		this->n[1][i] = b[i];
	}
	for (int i = 0; i < SIZEM3; i++) {
		this->n[2][i] = c[i];
	}
}

cgmath::vec3 & cgmath::mat3::operator[](int column)
{
	vec3 * a = reinterpret_cast<vec3*>(this->n[column]);
	return *a;
}

const cgmath::vec3 & cgmath::mat3::operator[](int column) const
{
	const vec3 * a = reinterpret_cast<const vec3*>(this->n[column]);
	return *a;
}

bool cgmath::mat3::operator==(const mat3 & m) const
{
	for (int i = 0; i < SIZEM3; i++) {
		for (int j = 0; j < SIZEM3; j++) {
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

float cgmath::mat3::determinant(const mat3 & m)
{
	return ((m.n[0][0]*m.n[1][1]*m.n[2][2]) + (m.n[0][1] * m.n[1][2] * m.n[2][0]) + (m.n[0][2] * m.n[1][0] * m.n[2][1]) - (m.n[0][2] * m.n[1][1] * m.n[2][0]) - (m.n[0][1] * m.n[1][0] * m.n[2][2]) - (m.n[0][0] * m.n[1][2] * m.n[2][1]));
}

cgmath::mat3 cgmath::mat3::inverse(const mat3 & m)
{
	vec3 v((m.n[1][1] * m.n[2][2] - m.n[2][1] * m.n[1][2]) / m.determinant(m), -1*(m.n[0][1] * m.n[2][2] - m.n[2][1] * m.n[0][2]) / m.determinant(m), (m.n[0][1] * m.n[1][2] - m.n[1][1] * m.n[0][2]) / m.determinant(m));
	vec3 w(-1*(m.n[1][0] * m.n[2][2] - m.n[2][0] * m.n[1][2]) / m.determinant(m), (m.n[0][0] * m.n[2][2] - m.n[2][0] * m.n[0][2]) / m.determinant(m), -1*(m.n[0][0] * m.n[1][2] - m.n[1][0] * m.n[0][2]) / m.determinant(m));
	vec3 x((m.n[1][0] * m.n[2][1] - m.n[2][0] * m.n[1][1]) / m.determinant(m), -1*(m.n[0][0] * m.n[2][1] - m.n[2][0] * m.n[0][1]) / m.determinant(m), (m.n[0][0] * m.n[1][1] - m.n[1][0] * m.n[0][1]) / m.determinant(m));
	return mat3(v,w,x);
}

cgmath::mat3 cgmath::mat3::transpose(const mat3 & m)
{
	vec3 v(m.n[0][0], m.n[1][0], m.n[2][0]);
	vec3 w(m.n[0][1], m.n[1][1], m.n[2][1]);
	vec3 x(m.n[0][2], m.n[1][2], m.n[2][2]);
	return mat3(v,w,x);
}

std::ostream & cgmath::operator<<(std::ostream & os, const mat3 & m)
{
	for (int i = 0; i < SIZEM3; i++) {
		for (int j = 0; j < SIZEM3; j++) {
			os << m.n[j][i];
			if (j!=SIZEM3-1) {
				os << " ";
			}
		}
		if (i!=SIZEM3-1) {
			os << "\n";
		}
	}
	return os;
}
