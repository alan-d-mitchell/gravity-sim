#ifndef VECTOR_H
#define VECTOR_H

// 3D vector structure
typedef struct {
    float x, y, z;
} Vec3;

// Vector operations
Vec3 vec3_add(Vec3 a, Vec3 b);
Vec3 vec3_sub(Vec3 a, Vec3 b);
Vec3 vec3_mul(Vec3 a, float scalar);
Vec3 vec3_div(Vec3 a, float scalar);
float vec3_dot(Vec3 a, Vec3 b);
Vec3 vec3_cross(Vec3 a, Vec3 b);
float vec3_length(Vec3 a);
Vec3 vec3_normalize(Vec3 a);
Vec3 vec3_zero(void);

#endif /* VECTOR_H */