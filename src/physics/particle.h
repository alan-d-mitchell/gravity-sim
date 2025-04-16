#ifndef PARTICLE_H
#define PARTICLE_H

#include "../utils/vector.h"

typedef struct {
    Vec3 position;     // Position in 3D space
    Vec3 velocity;     // Velocity vector
    Vec3 acceleration; // Acceleration vector
    float mass;        // Mass of the particle
    float radius;      // Visual radius for rendering
    Vec3 color;        // RGB color for rendering
} Particle;

// Initialize a particle with given properties
void particle_init(Particle *p, Vec3 pos, Vec3 vel, float mass, float radius, Vec3 color);

// Reset forces acting on the particle
void particle_reset_forces(Particle *p);

// Update particle state (called after forces are calculated)
void particle_update(Particle *p, float dt);

#endif /* PARTICLE_H */