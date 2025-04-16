#include "gravity.h"
#include "../utils/vector.h"
#include <math.h>

// Apply gravitational force between two particles
void apply_gravity(Particle *p1, Particle *p2) {
    // Calculate distance vector
    Vec3 r = {
        p2->position.x - p1->position.x,
        p2->position.y - p1->position.y,
        p2->position.z - p1->position.z
    };
    
    // Calculate squared distance
    float dist_sq = r.x * r.x + r.y * r.y + r.z * r.z;
    
    // Add softening parameter to prevent extreme forces at very close distances
    float softening = 1e-5f;
    dist_sq += softening;
    
    // Calculate distance
    float dist = sqrtf(dist_sq);
    
    // Normalize distance vector
    Vec3 dir = {
        r.x / dist,
        r.y / dist,
        r.z / dist
    };
    
    // Calculate gravitational force magnitude: F = G * m1 * m2 / r^2
    float force_mag = G * p1->mass * p2->mass / dist_sq;
    
    // Apply acceleration to each particle (F = ma, so a = F/m)
    float a1 = force_mag / p1->mass;
    float a2 = force_mag / p2->mass;
    
    // Update accelerations for both particles
    p1->acceleration.x += dir.x * a1;
    p1->acceleration.y += dir.y * a1;
    p1->acceleration.z += dir.z * a1;
    
    p2->acceleration.x -= dir.x * a2;
    p2->acceleration.y -= dir.y * a2;
    p2->acceleration.z -= dir.z * a2;
}

// Apply gravitational forces from all other particles
void apply_gravity_system(Particle *particles, int count, int current_index) {
    Particle *current = &particles[current_index];
    
    for (int i = 0; i < count; i++) {
        if (i == current_index) continue; // Skip self
        
        apply_gravity(current, &particles[i]);
    }
}

void apply_central_gravity(Particle *particle, Vec3 center_pos, float center_mass) {
    // Calculate distance vector
    Vec3 r = {
        center_pos.x - particle->position.x,
        center_pos.y - particle->position.y,
        center_pos.z - particle->position.z
    };
    
    // Calculate squared distance
    float dist_sq = r.x * r.x + r.y * r.y + r.z * r.z;
    
    // Add softening parameter to prevent extreme forces at very close distances
    float softening = 1e-5f;
    dist_sq += softening;
    
    // Calculate distance
    float dist = sqrtf(dist_sq);
    
    // Normalize distance vector
    Vec3 dir = {
        r.x / dist,
        r.y / dist,
        r.z / dist
    };
    
    // Calculate gravitational force magnitude: F = G * m1 * m2 / r^2
    float force_mag = G * particle->mass * center_mass / dist_sq;
    
    // Apply acceleration to particle (F = ma, so a = F/m)
    float acc = force_mag / particle->mass;
    
    // Update acceleration for the particle
    particle->acceleration.x += dir.x * acc;
    particle->acceleration.y += dir.y * acc;
    particle->acceleration.z += dir.z * acc;
}