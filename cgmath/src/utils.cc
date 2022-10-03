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