#ifndef GRAVITY_H
#define GRAVITY_H

#include "particle.h"

// Gravitational constant (can be adjusted for simulation scale)
#define G 6.67430e-11f

// Apply gravitational force between two particles
void apply_gravity(Particle *p1, Particle *p2);

// Apply gravitational force between a particle and all other particles in the system
void apply_gravity_system(Particle *particles, int count, int current_index);

// Optional: Apply gravity from a central massive body (e.g., sun in a solar system)
void apply_central_gravity(Particle *particle, Vec3 center_pos, float center_mass);

// Optional: Apply gravitational force with Barnes-Hut algorithm for optimization
// This would require additional structures like octrees
// void apply_barnes_hut_gravity(Particle *particle, Octree *root);

#endif /* GRAVITY_H */
