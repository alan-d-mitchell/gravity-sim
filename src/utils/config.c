#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// Initialize configuration with default values
void config_init(SimConfig *config) {
    // Window settings
    config->window_width = 1280;
    config->window_height = 720;
    config->window_title = "Advanced Gravity Simulation";
    
    // Simulation settings
    config->max_particles = 1000;
    config->time_step = 0.001f; // 1ms
    config->integration_method = 1; // Verlet integration
    
    // Particle settings
    config->particle_min_mass = 100.0f;
    config->particle_max_mass = 1000.0f;
    config->particle_min_radius = 0.1f;
    config->particle_max_radius = 0.5f;
    
    // Central body (e.g., sun in solar system)
    config->enable_central_body = 1;
    config->central_body_mass = 1.0e6f; // Much more massive
    config->central_body_position = (Vec3){0.0f, 0.0f, 0.0f};
    
    // Collision settings
    config->enable_collision = 1;
    config->collision_damping = 0.8f; // Energy loss in collisions
    
    // Space boundaries
    config->enable_bounded_space = 1;
    config->space_min = (Vec3){-100.0f, -100.0f, -100.0f};
    config->space_max = (Vec3){100.0f, 100.0f, 100.0f};
    
    // Shader paths
    config->vertex_shader_path = "shaders/vertex.glsl";
    config->fragment_shader_path = "shaders/fragment.glsl";
}

// Load configuration from file (simple implementation)
int config_load_from_file(SimConfig *config, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open configuration file: %s\n", filename);
        return 0;
    }
    
    // Read configuration values (very basic implementation)
    // In a real application, you'd want a more robust parsing mechanism
    fscanf(file, "window_width: %d\n", &config->window_width);
    fscanf(file, "window_height: %d\n", &config->window_height);
    // Read other parameters...
    
    fclose(file);
    return 1;
}

// Create initial particles with random properties
void create_initial_particles(SimConfig *config, Particle *particles) {
    // Seed the random number generator
    srand(time(NULL));
    
    for (int i = 0; i < config->max_particles; i++) {
        // Random position within space bounds
        Vec3 pos = {
            (float)rand() / RAND_MAX * (config->space_max.x - config->space_min.x) + config->space_min.x,
            (float)rand() / RAND_MAX * (config->space_max.y - config->space_min.y) + config->space_min.y,
            (float)rand() / RAND_MAX * (config->space_max.z - config->space_min.z) + config->space_min.z
        };
        
        // Random velocity (relatively small)
        Vec3 vel = {
            ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f,
            ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f,
            ((float)rand() / RAND_MAX * 2.0f - 1.0f) * 5.0f
        };
        
        // Random mass
        float mass = (float)rand() / RAND_MAX * (config->particle_max_mass - config->particle_min_mass) + config->particle_min_mass;
        
        // Radius based on mass (optional)
        float radius = (float)rand() / RAND_MAX * (config->particle_max_radius - config->particle_min_radius) + config->particle_min_radius;
        
        // Random color
        Vec3 color = {
            (float)rand() / RAND_MAX,
            (float)rand() / RAND_MAX,
            (float)rand() / RAND_MAX
        };
        
        // Initialize the particle
        particle_init(&particles[i], pos, vel, mass, radius, color);
    }
    
    // If central body is enabled, make the first particle the central body
    if (config->enable_central_body) {
        particles[0].position = config->central_body_position;
        particles[0].velocity = (Vec3){0.0f, 0.0f, 0.0f};
        particles[0].mass = config->central_body_mass;
        particles[0].radius = config->particle_max_radius * 5.0f; // Larger radius
        particles[0].color = (Vec3){1.0f, 1.0f, 0.0f}; // Yellow color for "sun"
    }
}