#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <algorithm>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(double min = 0.0, double max = 1.0) {
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    //return distribution(generator);
    return std::clamp(distribution(generator), min, max);
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 1));
}

// Common Headers

#include "ray.h"
#include "vec3.h"