#ifndef INTEGRATION_H
#define INTEGRATION_H

#include "particle.h"

// Simple Euler integration
void euler_integrate(Particle *p, float dt);

// Velocity Verlet integration (more accurate)
void verlet_integrate(Particle *p, float dt);

// Runge-Kutta 4th order integration (most accurate)
void rk4_integrate(Particle *p, float dt);

// Update the entire particle system using the selected integration method
void update_particle_system(Particle *particles, int count, float dt, int integration_method);

#endif /* INTEGRATION_H */