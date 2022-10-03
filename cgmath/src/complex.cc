#include "complex.h"

cgmath::complex::complex() : real(0.0f), imag(0.0f){
}

cgmath::complex::complex(float real, float imag) : real(real), imag(imag) {
	//this->real = real; //this es un apuntador
	//this->imag = imag;
}

void cgmath::complex::print() const {
	std::cout << real << " + " << imag << "i" << std::endl;
}

//paso por referencia
void cgmath::complex::add(const complex & other)
{
	real += other.real;
	imag += other.imag;
}

cgmath::complex& cgmath::complex::operator+=(const complex & other)
{
	real += other.real;
	imag += other.imag;

	return *this;//referencia * a la izquierda de un objeto obtenemos referencia al objeto
}

cgmath::complex cgmath::complex::add(const complex & a, const complex & b)
{
	/*cgmath::complex c;
	c += a;
	c += b;
	return c;*/
	return cgmath::complex(a.real + b.real, a.imag + b.imag);
}
