#include "particle.h"
#include <stdlib.h>

void particle_init(Particle *p, Vec3 pos, Vec3 vel, float mass, float radius, Vec3 color) {
    p->position = pos;
    p->velocity = vel;
    p->mass = mass;
    p->radius = radius;
    p->color = color;
    
    // Initialize acceleration to zero
    p->acceleration = (Vec3){0.0f, 0.0f, 0.0f};
}

void particle_reset_forces(Particle *p) {
    // Reset acceleration to zero before calculating new forces
    p->acceleration = (Vec3){0.0f, 0.0f, 0.0f};
}

void particle_update(Particle *p, float dt) {
    // This is a simple Euler integration
    // For actual simulation, use the integration methods from integration.c
    p->velocity.x += p->acceleration.x * dt;
    p->velocity.y += p->acceleration.y * dt;
    p->velocity.z += p->acceleration.z * dt;
    
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;
    p->position.z += p->velocity.z * dt;
}
