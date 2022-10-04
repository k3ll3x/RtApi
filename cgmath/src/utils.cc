#include "utils.h"

float cgmath::utils::rand_flt(float a, float b){
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int cgmath::utils::rand_int(int a, int b){
    return a + (rand() % ( b - a + 1 ));
}

double cgmath::utils::map(double s, double a1, double a2, double b1, double b2){
	return b1 + (s-a1)*(b2-b1)/(a2-a1);
}