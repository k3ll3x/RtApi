#pragma once

#include <iostream>

namespace cgmath {
	class complex {
	public:
		//This two variables are stored in memory (contigua) real y img una a lado de la otra en memoria
		//Solo las variables se almacenan, los metodos no
		float real;
		float imag;

		complex();
		complex(float real, float imag);

		//manera tradicional de imprimir una representacion de esta clase
		void print() const; //metodo constante, no modifica el estado del objeto, no modifica las variables

		//manera de hacerlo de C++, utilizando sobrecarga de operadores
		friend inline std::ostream& operator<< (std::ostream& os, const complex& c) {
			os << c.real << " + " << c.imag << "i";
			return os;
		}

		void add(const complex& other);

		complex& operator+=(const complex& other);

		static complex add(const complex& a, const complex& b);
	};

	inline complex operator+(const complex& a, const complex& b) {
		return complex(a.real+b.real,a.imag+b.imag);
	}
}