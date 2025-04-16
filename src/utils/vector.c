#include "vector.h"
#include <math.h>

Vec3 vec3_add(Vec3 a, Vec3 b) {
    Vec3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    Vec3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}

Vec3 vec3_mul(Vec3 a, float scalar) {
    Vec3 result = {a.x * scalar, a.y * scalar, a.z * scalar};
    return result;
}

Vec3 vec3_div(Vec3 a, float scalar) {
    if (scalar == 0.0f) return a; // Prevent division by zero
    float inv_scalar = 1.0f / scalar;
    Vec3 result = {a.x * inv_scalar, a.y * inv_scalar, a.z * inv_scalar};
    return result;
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 vec3_cross(Vec3 a, Vec3 b) {
    Vec3 result = {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

float vec3_length(Vec3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vec3 vec3_normalize(Vec3 a) {
    float length = vec3_length(a);
    if (length < 1e-8f) return a; // Prevent division by near-zero
    
    Vec3 result = {
        a.x / length,
        a.y / length,
        a.z / length
    };
    return result;
}

Vec3 vec3_zero(void) {
    Vec3 result = {0.0f, 0.0f, 0.0f};
    return result;
}
