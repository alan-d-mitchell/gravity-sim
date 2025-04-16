/* src/physics/integration.c */
#include "integration.h"
#include <stdlib.h>

// Euler integration (simplest but least accurate)
void euler_integrate(Particle *p, float dt) {
    // Update velocity based on acceleration
    p->velocity.x += p->acceleration.x * dt;
    p->velocity.y += p->acceleration.y * dt;
    p->velocity.z += p->acceleration.z * dt;
    
    // Update position based on velocity
    p->position.x += p->velocity.x * dt;
    p->position.y += p->velocity.y * dt;
    p->position.z += p->velocity.z * dt;
}

// Velocity Verlet integration (better accuracy)
void verlet_integrate(Particle *p, float dt) {
    // Store old acceleration for second half of the update
    Vec3 old_acc = p->acceleration;
    
    // Update position based on velocity and half acceleration
    p->position.x += p->velocity.x * dt + 0.5f * old_acc.x * dt * dt;
    p->position.y += p->velocity.y * dt + 0.5f * old_acc.y * dt * dt;
    p->position.z += p->velocity.z * dt + 0.5f * old_acc.z * dt * dt;
    
    // At this point, we need new accelerations based on the updated positions
    // This is typically done by recalculating forces, which happens outside this function
    
    // Update velocity using average of old and new acceleration
    p->velocity.x += 0.5f * (old_acc.x + p->acceleration.x) * dt;
    p->velocity.y += 0.5f * (old_acc.y + p->acceleration.y) * dt;
    p->velocity.z += 0.5f * (old_acc.z + p->acceleration.z) * dt;
}

// Runge-Kutta 4th order integration (most accurate but computationally expensive)
void rk4_integrate(Particle *p, float dt) {
    // This is a simplified RK4 implementation
    // A full implementation would require storing the state at multiple steps
    // and evaluating forces multiple times, which is complex
    
    // k1 = f(y_n)
    Vec3 k1_vel = p->acceleration;
    Vec3 k1_pos = p->velocity;
    
    // k2 = f(y_n + dt/2 * k1)
    Vec3 k2_pos = {
        p->velocity.x + 0.5f * dt * k1_vel.x,
        p->velocity.y + 0.5f * dt * k1_vel.y,
        p->velocity.z + 0.5f * dt * k1_vel.z
    };
    // For k2_vel, we would need to recalculate acceleration at the new position
    // This is typically done outside this function
    Vec3 k2_vel = p->acceleration; // Simplification
    
    // k3 = f(y_n + dt/2 * k2)
    Vec3 k3_pos = {
        p->velocity.x + 0.5f * dt * k2_vel.x,
        p->velocity.y + 0.5f * dt * k2_vel.y,
        p->velocity.z + 0.5f * dt * k2_vel.z
    };
    // Again, we would need to recalculate acceleration
    Vec3 k3_vel = p->acceleration; // Simplification
    
    // k4 = f(y_n + dt * k3)
    Vec3 k4_pos = {
        p->velocity.x + dt * k3_vel.x,
        p->velocity.y + dt * k3_vel.y,
        p->velocity.z + dt * k3_vel.z
    };
    // One more acceleration recalculation
    Vec3 k4_vel = p->acceleration; // Simplification
    
    // Update position: y_{n+1} = y_n + dt/6 * (k1 + 2*k2 + 2*k3 + k4)
    p->position.x += dt / 6.0f * (k1_pos.x + 2.0f * k2_pos.x + 2.0f * k3_pos.x + k4_pos.x);
    p->position.y += dt / 6.0f * (k1_pos.y + 2.0f * k2_pos.y + 2.0f * k3_pos.y + k4_pos.y);
    p->position.z += dt / 6.0f * (k1_pos.z + 2.0f * k2_pos.z + 2.0f * k3_pos.z + k4_pos.z);
    
    // Update velocity
    p->velocity.x += dt / 6.0f * (k1_vel.x + 2.0f * k2_vel.x + 2.0f * k3_vel.x + k4_vel.x);
    p->velocity.y += dt / 6.0f * (k1_vel.y + 2.0f * k2_vel.y + 2.0f * k3_vel.y + k4_vel.y);
    p->velocity.z += dt / 6.0f * (k1_vel.z + 2.0f * k2_vel.z + 2.0f * k3_vel.z + k4_vel.z);
}

// Update the entire particle system
void update_particle_system(Particle *particles, int count, float dt, int integration_method) {
    // First, reset all forces
    for (int i = 0; i < count; i++) {
        particle_reset_forces(&particles[i]);
    }
    
    // Calculate gravitational forces between all pairs of particles
    // This is O(n²) complexity - for large simulations, optimization would be needed
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            apply_gravity(&particles[i], &particles[j]);
        }
    }
    
    // Update all particles using the selected integration method
    for (int i = 0; i < count; i++) {
        switch (integration_method) {
            case 0:
                euler_integrate(&particles[i], dt);
                break;
            case 1:
                verlet_integrate(&particles[i], dt);
                break;
            case 2:
                rk4_integrate(&particles[i], dt);
                break;
            default:
                euler_integrate(&particles[i], dt);
        }
    }
}