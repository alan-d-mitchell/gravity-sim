#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "camera.h"
#include "../physics/particle.h"
#include "../utils/config.h"

typedef struct {
    GLFWwindow* window;
    int window_width;
    int window_height;
    
    Shader shader;
    Camera camera;
    
    GLuint vao;  // Vertex Array Object
    GLuint vbo;  // Vertex Buffer Object
    
    double last_frame_time;
    float delta_time;
    
    // Mouse input tracking
    double last_mouse_x;
    double last_mouse_y;
    int first_mouse;
    
    // Keyboard state
    int keys[1024];
    
    // Simulation state
    int paused;
    int step_mode;
    int single_step;
} Renderer;

// Initialize the renderer
int renderer_init(Renderer *renderer, SimConfig *config);

// Main rendering loop
void renderer_main_loop(Renderer *renderer, Particle *particles, int particle_count, SimConfig *config);

// Render a single frame
void renderer_render_frame(Renderer *renderer, Particle *particles, int particle_count);

// Process input
void renderer_process_input(Renderer *renderer);

// Update delta time
void renderer_update_time(Renderer *renderer);

// Cleanup and free resources
void renderer_cleanup(Renderer *renderer);

// Callback functions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif /* RENDERER_H */