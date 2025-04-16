#ifndef CONFIG_H
#define CONFIG_H

#include "vector.h"
#include "../physics/particle.h"

typedef struct {
    int window_width;
    int window_height;
    const char *window_title;
    
    int max_particles;
    float time_step;
    int integration_method; // 0: Euler, 1: Verlet, 2: RK4
    
    float particle_min_mass;
    float particle_max_mass;
    float particle_min_radius;
    float particle_max_radius;
    
    int enable_central_body;
    float central_body_mass;
    Vec3 central_body_position;
    
    int enable_collision;
    float collision_damping;
    
    int enable_bounded_space;
    Vec3 space_min;
    Vec3 space_max;
    
    const char *vertex_shader_path;
    const char *fragment_shader_path;
} SimConfig;

// Initialize configuration with default values
void config_init(SimConfig *config);

// Load configuration from file (optional)
int config_load_from_file(SimConfig *config, const char *filename);

// Create initial particles based on configuration
void create_initial_particles(SimConfig *config, Particle *particles);

#endif /* CONFIG_H */