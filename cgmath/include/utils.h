#pragma once

#include <stdlib.h>

namespace cgmath {
	class utils {
	public:
		static float rand_flt(float a, float b);
		static int rand_int(int a, int b);
		static double map(double s, double a1, double a2, double b1, double b2);
	};
}