/* src/main.c */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "physics/particle.h"
#include "physics/gravity.h"
#include "physics/integration.h"
#include "render/renderer.h"
#include "utils/config.h"

int main(int argc, char *argv[]) {
    // Seed random number generator
    srand(time(NULL));
    
    // Initialize configuration
    SimConfig config;
    config_init(&config);
    
    // Parse command line arguments and/or configuration file
    if (argc > 1) {
        if (config_load_from_file(&config, argv[1])) {
            printf("Loaded configuration from file: %s\n", argv[1]);
        } else {
            printf("Failed to load configuration file, using defaults\n");
        }
    }
    
    // Create particles
    Particle *particles = (Particle*)malloc(config.max_particles * sizeof(Particle));
    if (!particles) {
        fprintf(stderr, "Failed to allocate memory for particles\n");
        return -1;
    }
    
    // Initialize particles
    create_initial_particles(&config, particles);
    printf("Created %d particles\n", config.max_particles);
    
    // Initialize renderer
    Renderer renderer;
    if (!renderer_init(&renderer, &config)) {
        fprintf(stderr, "Failed to initialize renderer\n");
        free(particles);
        return -1;
    }
    
    printf("Starting simulation with:\n");
    printf("- %d particles\n", config.max_particles);
    printf("- Time step: %f\n", config.time_step);
    printf("- Integration method: %d\n", config.integration_method);
    
    if (config.enable_central_body) {
        printf("- Central body enabled with mass %e\n", config.central_body_mass);
    }
    
    // Main loop
    renderer_main_loop(&renderer, particles, config.max_particles, &config);
    
    // Cleanup
    renderer_cleanup(&renderer);
    free(particles);
    
    printf("Simulation completed\n");
    return 0;
}